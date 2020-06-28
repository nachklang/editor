#pragma once

class LevelController: public QObject
{
    Q_OBJECT

public:
    void CreateNewLevel();

    void OpenExistedLevel();

    void SaveLevel();
};
