#include "wombatframework.h"

WombatFramework::WombatFramework(WombatVariable* wombatvariable)
{
    wombatptr = wombatvariable;
}
WombatFramework::~WombatFramework()
{
}
void WombatFramework::BuildEvidenceModel()
{
    /*
    QFutureWatcher<void> watcher;
    std::vector<TskSchedulerQueue::task_struct* > tmpvector;
    watcher.setFuture(QtConcurrent::map(&tmpvector, &SleuthKitPlugin::TaskMap));
    //watcher.setFuture(QtConcurrent::map(&((std::vector<TskSchedulerQueue::task_struct*>)scheduler.mapvector), &SleuthKitPlugin::TaskMap));
    watcher.waitForFinished();
    */
    // QT CONCURRENT TEST
}
