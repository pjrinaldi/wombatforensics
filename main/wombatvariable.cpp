#include "wombatvariable.h"

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

int WombatVariable::GetImageID()
{
    return imageid;
}

int WombatVariable::GetAnalysisType()
{
    return analysistype;
}

void WombatVariable::SetCaseID(int caseID)
{
    caseid = caseID;
}

void WombatVariable::SetImageID(int imageID)
{
    imageid = imageID;
}

void WombatVariable::SetAnalysisType(int analysisType)
{
    analysistype = analysisType;
}
