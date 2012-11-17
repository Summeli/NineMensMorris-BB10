#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

class Settings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool computerWhite READ computerWhite WRITE setComputerWhite NOTIFY computerWhiteChanged)
    Q_PROPERTY(bool computerBlack READ computerBlack WRITE setComputerBlack NOTIFY computerBlackChanged)
    Q_PROPERTY(bool faceToFace READ faceToFace WRITE setFaceToFace NOTIFY faceToFaceChanged)
    Q_PROPERTY(int difficulty READ difficulty WRITE setDifficulty NOTIFY difficultyChanged)
    Q_PROPERTY(bool fullScreen READ fullScreen WRITE setFullScreen NOTIFY fullScreenChanged)
public:
    explicit Settings(QObject *parent = 0);

    Q_INVOKABLE void load();
    Q_INVOKABLE void save();

    void setComputerWhite(bool computer);
    bool computerWhite() const;
    void setComputerBlack(bool computer);
    bool computerBlack() const;
    void setFaceToFace(bool faceToFace);
    bool faceToFace() const;
    void setDifficulty(int difficulty);
    int difficulty() const;
    void setFullScreen(bool fullScreen);
    bool fullScreen() const;

signals:
    void computerWhiteChanged();
    void computerBlackChanged();
    void faceToFaceChanged();
    void difficultyChanged();
    void fullScreenChanged();

public slots:

private:
    QSettings m_settings;
    bool m_computerWhite;
    bool m_computerBlack;
    bool m_faceToFace;
    int m_difficulty;
    bool m_fullScreen;
};

#endif // SETTINGS_H
