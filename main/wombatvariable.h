#ifndef WOMBATVARIABLE_H
#define WOMBATVARIABLE_H

class WombatVariable
{
public:
    WombatVariable();
    int GetCaseID();
    int GetJobID();
    int GetEvidenceID();
    int GetJobType();
    void SetCaseID(int caseID);
    void SetJobID(int jobID);
    void SetEvidenceID(int evidenceID);
    void SetJobType(int jobType);
    ~WombatVariable();

    // ADD ENUMS FOR JOBSTATUS, JOBTYPE, ETC TO SIMPLIFY INTEGER ASSOCIATION AND READABILITY IN CODE
    enum JobStatus
    {
        failed = 0,
        finish = 1,
        finisherror = 2
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
};
#endif // WOMBATVARIABLE_H
