#pragma once

#include <QWidget>

extern "C" {
#include <mpv/client.h>
}

class MpvPlayer : public QObject {
    Q_OBJECT
public:
    explicit MpvPlayer(QWidget* container, QObject* parent = nullptr);
    ~MpvPlayer();
    void        handleMpvEvent();
    mpv_handle* getMpvHandle();

    void setUrl(const QString& url);

    void play();

signals:
    void onMpvEventChanged();

private:
    QWidget*    container_;
    mpv_handle* mpv_;

    QByteArray url_;
};
