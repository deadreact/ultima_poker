#include <precompiled.h>
#include "page_UploadAvatar.h"
#include "ui_page_UploadAvatar.h"
#include <const.h>

namespace ns_page
{
    UploadAvatar::UploadAvatar(QWidget *parent)
        : QWidget(parent)
        , m_ui(new Ui::UploadAvatar)
        , m_selectedFragment()
    {
        m_ui->setupUi(this);
    }

    UploadAvatar::~UploadAvatar()
    {
        delete m_ui;
    }

    Avatar UploadAvatar::getAvatar() const {
        return Avatar(m_selectedFragment);
    }

    void UploadAvatar::init(const QPixmap &pixmap)
    {
        m_ui->pictureEditor->setPixmap(pixmap);
        m_ui->pictureEditor->setSizeOfFrame(pixmap.size());
        connect(m_ui->pictureEditor, &PixmapEditor::selectionChanged, this, [this](QRectF) {
            m_selectedFragment = m_ui->pictureEditor->selectedFragment();
            if (m_selectedFragment.width() > 300) {
                m_selectedFragment = m_selectedFragment.scaled(244, 244, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            }
            m_ui->pictureEditorResult->setAvatar(getAvatar());
        }, Qt::UniqueConnection);
        m_ui->pictureEditor->init();
    }

} //namespace ns_page
