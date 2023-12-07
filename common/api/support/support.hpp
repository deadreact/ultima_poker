#pragma once

#include "../api_common.h"
#include <boost/cstdint.hpp>

namespace gbp {
namespace ns_api {
namespace ns_support {

GBP_DECLARE_ENUM(
	 e_user_type
	,boost::uint8_t
	,inline
	,
	(is_user)
	(is_admin)
)

GBP_DECLARE_ENUM_SIMPLE(
	 e_status
	,inline
	,
	(ok                      , 1U<<0)
	(chat_is_not_exists      , 1U<<1)
	(user_already_in_chat    , 1U<<2)
	(user_not_in_chat        , 1U<<3)
	(bad_username_or_password, 1U<<4)
	(already_logged_in       , 1U<<5)
	(not_logged_in           , 1U<<6)
	(admin_blocked           , 1U<<7)
	(chat_blocked            , 1U<<8)
	(support_blocked         , 1U<<9)
	(user_is_not_registered  , 1U<<10)
	// ...
	(unknown_error           , 1U<<31)
)
using e_status_type = std::underlying_type<e_status>::type;

GBP_DECLARE_ENUM(
	 e_download_stat
	,boost::uint8_t
	,inline
	,
	(ok)
	(bad_checksumm)
	(canceled_by_user)
	(canceled_by_admin)
	(already_in_queue)
	(unknown_error)
)

GBP_DECLARE_ENUM(
	 e_uset_status
	,boost::uint8_t
	,inline
	,
	(logged_in)
	(logged_out)
)

GBP_DECLARE_TYPE(
	file_info
	,
	(name        , (std::string)   )
	(size        , (std::uint32_t) )
	(sha1        , (std::string)   )
	(piece       , (std::string)   )
	(piece_offset, (std::uint32_t) )
	(piece_size  , (std::uint32_t) )
)

/***************************************************************************/

namespace ns_request { // user -> server

struct support {
	virtual ~support() {}

	// login/logout вызываются только админом.
	virtual void login(
		 const std::string &nickname
		,const std::string &password_sha1
	) = 0;
	virtual void logout() = 0;

	/* лоббя, при запуске подключается к саппорту. при этом, лоббя сразу зовет 'send_message()'
	 * со всеми тремя пустыми строками. таким образом сервер понимает, что подключился нелогиненный юзер
	 * и генерит для него уникальный никнейм.
	 * далее, когда юзер логинится в лоббю, она зовет 'send_message()' с установленным только 'nickname'.
	 * так сервер переназначает сессии сгенеренный никнейм тем, что прислала лоббя.
	 * если после этого, юзер отлогинивается - лоббя снова зовет 'send_message()' со всеми пустыми строками
	 * и сервер снова генерит уникальный никнейм и снова переназначает его сессии.
	 */
	virtual void send_message(
		 const std::string &nickname
		,const std::string &receiver_nickname // is empty() if 'user_type' == is_user
		,const std::string &msg
	) = 0;

	/* при отправке аттача юзером на сервер(attachments_req()), он использует поля 'name', 'size' и 'sha1'.
	 * сервер только переправляет этот запрос админу(attachments_rep()) для получения разрешения.
	 * админ может разрешить/запретить как каждый файл по отдельности, так и все файлы одновременно.
	 * после этого, админ отправляет серверу информацию о своем решении(attachments_req())
	 * (т.е. в векторе содержатся только те элементы file_info, которые админ разрешил загрузить), а сервер
	 * пересылает ее юзеру(attachments_rep()).
	 * таким образом, если юзеру приходит ответ(attachments_rep()) и список файлов пуст - значит админ отменил выгрузку юзером файлов.
	 *
	 * после того как админ утвердил список отправляемых файлов, этот список помимо отправки юзеру,
	 * запоминается еще и сессией юзера для того, чтоб сессия могла сама запрашивать файлы у юзера.
	 * это нужно для того, чтоб предотвратить ситуации, когда юзер по факту присылает не все файлы, или
	 * пытается прислать другие файлы.
	 * в период между запросом юзера о выгрузке файлов и до окончания выгрузки - юзеру запрещается звать 'attachments_req(...)'.
	 *
	 * процесс загрузки файлов сервером.
	 * сервер, для каждого файла вызывает 'download_piece_req(file_info)' такое кол-во раз, сколько необходимо для загрузки файла.
	 * сервер так же передает смещение и размер "куска". юзер, в ответ вызывает 'download_piece_rep()', передав аргументом запрошенный сервером "кусок".
	 * по завершению загрузки файла, или в случае возникновения ошибки - сервер вызывает 'download_piece_finished()'
	 * с соответствующим 'e_download_piece' кодом.
	 * по окончании выгрузки юзером файла, сервер для админа вызывает 'download_finished()', для каждого файла.
	 *
	 * процесс загрузки файлов админом.
	 * админ, для каждого загруженного файла вызывает 'download_req()'. сервер, в ответ, вызывает 'download_rep()'.
	 * как только админ реально получил файл, он делает запрос следующего.
	 */
	virtual void attachments_req(
		 const std::vector<file_info> &attach
		,const std::string &receiver // is empty if 'user_type' == is_user
	) = 0;

	// called only by users
	virtual void download_piece_rep(const file_info &fi) = 0;

	// called only by support
	virtual void download_req(const std::string &nickname, const std::string &sha1) = 0;

	// called by lobby and support
	virtual void ping(gbp::ns_api::time_s posix_time) = 0;
};

} // ns ns_request

/***************************************************************************/

namespace ns_reply { // server -> user

struct support {
	virtual ~support() {}

	virtual void login(e_status_type stat, const std::string &real_name) = 0;
	virtual void logout(e_status_type stat) = 0;

	virtual void recv_message(
		 const std::string &nickname
		,const std::string &receiver // is empty if 'user_type' == is_user
		,const std::string &msg
		,gbp::ns_api::time_s dt
	) = 0;

	// attachments operations
	virtual void attachments_rep(
		 const std::vector<file_info> &attach
		,const std::string &receiver
	) = 0;

	// called only for users
	virtual void download_piece_req(const file_info &fi) = 0;
	virtual void download_piece_finished(const std::string &sha1, e_download_stat stat) = 0;

	// called only for support
	virtual void download_finished(const std::string &nickname, const std::string &sha1, e_download_stat stat) = 0;
	virtual void download_rep(const std::string &nickname, const file_info &file) = 0;

	// called only for support
	/*
	 * когда какой-либо юзер подключается - сервер сообщает саппорту о этом:
	 * std::pair<std::string, e_uset_status> = {"user nickname", e_uset_status::logged_in}
	 *
	 * если же юзер отключается - то сервер сообщает саппорту так:
	 * std::pair<std::string, e_uset_status> = {"user nickname", e_uset_status::logged_out}
	 *
	 * когда саппорт подключается - он сразу получает вектор пар всех юзеров, где
	 * 'e_uset_status' ровняется 'logged_in'.
	 */
	virtual void user_status_changed(const std::vector<std::pair<std::string, e_uset_status>> &ops) = 0;
};

} // ns ns_reply

/***************************************************************************/

} // ns ns_support
} // ns ns_api
} // ns gbp