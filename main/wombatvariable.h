#ifndef WOMBATVARIABLE_H
#define WOMBATVARIABLE_H

#include <QString>
#include <QList>
#include <QStringList>

class WombatVariable
{
public:
    WombatVariable();
    int GetCaseID();
    int GetJobID();
    int GetEvidenceID();
    int GetJobType();
    QList<QObject*> GetPlugins();
    QString GetCasesPath();
    QString GetCaseDirPath();
    QStringList GetEvidence();
    QString GetEvidenceDirPath();
    QString GetSettingsPath();
    QString GetDataPath();
    QString GetTmpFilePath();
    void SetCaseID(int caseID);
    void SetJobID(int jobID);
    void SetEvidenceID(int evidenceID);
    void SetJobType(int jobType);
    void SetPlugins(QList<QObject*> pluginList);
    void SetCasesPath(QString casesPath);
    void SetCaseDirPath(QString casePath);
    void AddEvidence(QString evidence);
    void SetEvidenceDirPath(QString evidenceDirPath);
    void SetSettingsPath(QString settingsPath);
    void SetDataPath(QString dataPath);
    void SetTmpFilePath(QString tmpFilePath);

    ~WombatVariable();

    // ADD ENUMS FOR JOBSTATUS, JOBTYPE, ETC TO SIMPLIFY INTEGER ASSOCIATION AND READABILITY IN CODE
    enum JobStatus
    {
        failed = 0,
        finish = 1,
        finisherror = 2,
        cancelled = 3
    };

    enum JobType
    {
        generic = 0,
        addevidence = 1,
        remedvidence = 2
    };

private:
    int caseid;
    int evidenceid;
    int jobtype;
    int jobid;
    QList<QObject*> plugins;
    QString casespath;
    QString casedirpath;
    QString evidencedirpath;
    QStringList evidence;
    QString settingspath;
    QString datapath;
    QString tmpfilepath;

};
#endif // WOMBATVARIABLE_H
