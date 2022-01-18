#pragma once

#include <QCoreApplication>
#include <QTimer>
#include <QWidget>

extern "C" {
#include <mpv/client.h>
}

class MpvPlayer : public QObject {
    Q_OBJECT;

public:
    explicit MpvPlayer(QWidget* container, QObject* parent = nullptr);
    ~MpvPlayer();
    void        handleMpvEvent();
    mpv_handle* getMpvHandle();

    void setPosition(double position);
    // 音频文件的长度，单位是秒
    double duration() const;
    // 添加媒体到播放列表末尾
    void addMedia(QString const& url);

signals:
    void onMpvEventChanged();
    // 以秒为单位的位置。可能会超出文件范围
    // https://mpv.io/manual/master/#command-interface-time-pos
    void positioinChanged(double position);

protected:
    void initMpvProps();

private:
    QTimer*     timer_;
    QWidget*    container_;
    mpv_handle* mpv_;
};
