#include "emailsender.h"
#include "../SMTPEmail/src/SmtpMime"
#include <QSettings>
#include <QtWidgets/QApplication>
#include <QMetaEnum>
#include <qdir.h>
//#include <sstream>
//#include <boost/format.hpp>
//#include <python2.7/Python.h>

namespace
{
    template <typename enumType>
    constexpr typename std::underlying_type<enumType>::type underlying_cast(enumType enumValue)
    {
        return static_cast<typename std::underlying_type<enumType>::type>(enumValue);
    }

    template <typename EnumType>
    QString enumToString(EnumType enumValue)
    {
        return QMetaEnum::fromType<EnumType>().valueToKey(underlying_cast(enumValue));
    }
}

namespace ns_email_sender
{
//    namespace ns_pycode
//    {
//        const char* get_python_script() {
//            static const char* pycode =
//R"(#!/usr/bin/env python -u
//import smtplib, os, sys
//from email.mime.multipart import MIMEMultipart
//from email.mime.base import MIMEBase
//from email.mime.text import MIMEText
//from email.utils import COMMASPACE, formatdate
//from email import encoders

//# port: 465/587
//def send_mail(send_from, send_to, subject, message, files, server, port, username, password):
//    if port == 465:
//        smtp = smtplib.SMTP_SSL(server, port)
//    elif port == 587:
//        smtp = smtplib.SMTP(server, port)
//        smtp.starttls()
//    else:
//        smtp = smtplib.SMTP(server, port)

//    smtp.set_debuglevel(False)
//    smtp.login(username, password)
//    idx = 0
//    for to in send_to:
//        print('idx={}, user={}'.format(idx, to))
//        msg = MIMEMultipart()
//        msg['From'] = send_from
//        msg['To'] = to
//        msg['Date'] = formatdate(localtime = True)
//        msg['Subject'] = subject
//        msg.attach(MIMEText(message, 'plain', 'utf-8'))
//        for f in files:
//            part = MIMEBase('application', 'octet-stream')
//            part.set_payload( open(f,"rb").read() )
//            encoders.encode_base64(part)
//            part.add_header('Content-Disposition', 'attachment; filename="{}"'.format(os.path.basename(f)))
//            msg.attach(part)
//        try:
//            smtp.sendmail(send_from, to, msg.as_string())
//        except smtplib.SMTPRecipientsRefused as ex:
//            print('SMTPRecipientsRefused exception: {}'.format(ex))
//        except smtplib.SMTPHeloError as ex:
//            print('SMTPHeloError exception: {}'.format(ex))
//        except smtplib.SMTPSenderRefused as ex:
//            print('SMTPSenderRefused exception: {}'.format(ex))
//        except smtplib.SMTPDataError as ex:
//            print('SMTPDataError exception: {}'.format(ex))
//        except:
//            print('Unexpected error: {}'.format(sys.exc_info()[0]))
//            raise
//        finally:
//            idx = idx+1

//    print('done')
//    smtp.quit()

//send_mail(
//     '%1%' # sender
//    ,[%2%] # receivers
//    ,'%3%' # subject
//    ,'''%4%''' # message
//    ,[%5%] # files
//    ,'%6%' # smtp server
//    ,%7%   # smtp port
//    ,'%8%' # smtp user
//    ,'%9%' # smtp password
//)
//)";

//            return pycode;
//        }


//        int verification(const message* msg, const char* config)
//        {
//            SMTPInfo settings(config);

//            const std::string receiver = "'" + settings.recipient.toStdString() + "'";
//            std::ostringstream os;
//            os << boost::format(get_python_script())
//                % settings.sender.toStdString()
//                % receiver
//                % settings.subject.toStdString()
//                % msg->text.toStdString()
//                % ("[" + msg->attachments.join(", ").toStdString() + "]")
//                % settings.smtp.toStdString()
//                % settings.port
//                % settings.user.toStdString()
//                % settings.pass.toStdString()
//                ;
////            Py_SetProgramName(argv[0]);
//            Py_Initialize();

//            int errcode = PyRun_SimpleString(os.str().c_str());
////            const bool ok = errcode == 0;
//            Py_Finalize();

//            return errcode;
//        }
//    } // namespace ns_pycode


    QString SMTPInfo::lastError;

    SMTPInfo::SMTPInfo(const char* filename)
    {
        if (QFile::exists(filename))
        {
            QSettings settings(filename, QSettings::IniFormat);

            settings.beginGroup("smtp_info");
            smtp      = settings.value("smtp_address", "mail.ultimapoker.com").toString();
            port      = settings.value("smtp_port", 587).value<quint16>();
            user      = settings.value("user", "testmail@ultimapoker.com").toString();
            pass      = settings.value("pass", "blablablaxyz").toString();
            sender    = settings.value("sender", "testmail@ultimapoker.com").toString();
            recipient = settings.value("recipient", "testmail@ultimapoker.com").toString();

            QString strconntype = settings.value("recipient", "testmail@ultimapoker.com").toString();
            conn_type = SmtpClient::TcpConnection;
            if (strconntype == "Ssl") {
                conn_type = SmtpClient::SslConnection;
            } else if (strconntype == "Tls") {
                conn_type = SmtpClient::TlsConnection;
            }
            subject   = "verification";
            settings.endGroup();
            lastError = "";
        }
        else
        {
            lastError = QString("File %0 not found").arg(filename);
        }

        QStringList lst;
        lst << *this;
        //qDebug() << lst;
    }

int verification(const message* msg, const char* config)
{
    SMTPInfo settings(config);
    SmtpClient::ConnectionType connType = SmtpClient::ConnectionType(settings.conn_type);

    SmtpClient smtp(settings.smtp, settings.port, connType);

    QObject::connect(&smtp, &SmtpClient::smtpError, [](SmtpClient::SmtpError e){
        //qDebug() << "SmtpError" << ::enumToString(e);
    });

    smtp.setUser(settings.user);
    smtp.setPassword(settings.pass);

    MimeMessage message;

    EmailAddress sender(settings.sender, "username");
    EmailAddress recipient(settings.recipient, "username");
    message.setSender(&sender);
    message.addRecipient(&recipient);
    message.setSubject(settings.subject);

    MimeText text(msg->text);

    message.addPart(&text);

    for (const QString& path: msg->attachments) {
        message.addPart(new MimeAttachment(new QFile(path)));
    }

    if (!smtp.connectToHost()) {
        SMTPInfo::lastError = "Failed to connect to host!";
        return -1;
    }

    if (!smtp.login()) {
        SMTPInfo::lastError = "Failed to login!";
        return -2;
    }

    QApplication::processEvents();
    if (!smtp.sendMail(message)) {
        SMTPInfo::lastError = "Failed to send mail!";
        return -3;
    }
//    qDebug() << settings;
    smtp.quit();

    return 0;
}

} //ns_email_sender

//QTextStream &operator<<(QTextStream &os, const ns_email_sender::SMTPInfo &info) {
//    static const char* pattern("SMTPInfo(smtp: %0, port: %1, user: %2, pass %3, sender: %4, recipient: %5, subject: %6");
//    QString str = QString(pattern).arg(info.smtp).arg(info.port).arg(info.user, info.pass, info.sender, info.recipient, info.subject);
//    if (!info.lastError.isNull() && !info.lastError.isEmpty()) {
//        os << QString("SMTPError(lastError:%0)").arg(info.lastError);
//    }
//    os << str;
//    return os;
//}
