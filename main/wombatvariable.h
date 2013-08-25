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
    enum 

private:
    int caseid;
    int evidenceid;
    int jobtype;
    int jobid;
};
#endif // WOMBATVARIABLE_H
