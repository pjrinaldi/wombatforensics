#ifndef WOMBATVARIABLE_H
#define WOMBATVARIABLE_H

class WombatVariable
{
public:
    WombatVariable();
    int GetCaseID();
    int GetImageID();
    int GetAnalysisType();
    void SetCaseID(int caseID);
    void SetImageID(int imageID);
    void SetAnalysisType(int analysisType);
    ~WombatVariable();

private:
    int caseid;
    int imageid;
    int analysistype;
};
#endif // WOMBATVARIABLE_H
