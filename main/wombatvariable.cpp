#include "wombatvariable.h"

// ADD ENUMS FOR JOBSTATUS, JOBTYPE, ETC TO SIMPLIFY INTEGER ASSOCIATION AND READABILITY IN CODE

WombatVariable::WombatVariable()
{
}

WombatVariable::~WombatVariable()
{
}

int WombatVariable::GetCaseID()
{
    return caseid;
}

int WombatVariable::GetJobID()
{
    return jobid;
}

int WombatVariable::GetEvidenceID()
{
    return evidenceid;
}

int WombatVariable::GetJobType()
{
    return jobtype;
}

void WombatVariable::SetCaseID(int caseID)
{
    caseid = caseID;
}

void WombatVariable::SetJobID(int jobID)
{
    jobid = jobID;
}

void WombatVariable::SetEvidenceID(int evidenceID)
{
    evidenceid = evidenceID;
}

void WombatVariable::SetJobType(int jobType)
{
    jobtype = jobType;
}
