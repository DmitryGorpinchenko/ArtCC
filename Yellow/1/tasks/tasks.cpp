#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <algorithm>
using namespace std;

enum class TaskStatus {
    NEW,
    IN_PROGRESS,
    TESTING,
    DONE
};

using TasksInfo = map<TaskStatus, int>;

class TeamTasks {
public:
    const TasksInfo& GetPersonTasksInfo(const string& person) const {
        return infos.at(person);
    }

    void AddNewTask(const string& person) {
        ++infos[person][TaskStatus::NEW];
    }

    tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string& person, int task_count) {
        static const auto next = [](TaskStatus s) { return static_cast<TaskStatus>(static_cast<int>(s) + 1); };
        auto& info = infos[person];
        TasksInfo diff, updated, untouched;
        for (const auto& [ status, count ] : info) {
            if (status != TaskStatus::DONE) {
                const auto moved = std::min(count, task_count);
                if (moved > 0) {
                    diff[status] -= moved;
                    diff[next(status)] += moved;
                    updated[next(status)] = moved;
                    task_count -= moved;
                }
                if (count > moved) {
                    untouched[status] = count - moved;
                }
            }
        }
        for (const auto& [ status, count ] : diff) {
            const auto new_count = info[status] + count;
            if (new_count > 0) {
                info[status] = new_count;
            } else {
                info.erase(status);
            }
        }
        return { updated, untouched };
    }
private:
    map<string, TasksInfo> infos;
};

void PrintTasksInfo(TasksInfo tasks_info) {
    cout << tasks_info[TaskStatus::NEW] << " new tasks" << ", "
         << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress" << ", "
         << tasks_info[TaskStatus::TESTING] << " tasks are being tested" << ", "
         << tasks_info[TaskStatus::DONE] << " tasks are done" << endl;
}

int main() {
    TeamTasks tasks;
    tasks.AddNewTask("Ilia");
    for (int i = 0; i < 3; ++i) {
        tasks.AddNewTask("Ivan");
    }
    cout << "Ilia's tasks: ";
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"));
    cout << "Ivan's tasks: ";
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));

    TasksInfo updated_tasks, untouched_tasks;
    
    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan", 2);
    cout << "Updated Ivan's tasks: ";
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: ";
    PrintTasksInfo(untouched_tasks);

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan", 2);
    cout << "Updated Ivan's tasks: ";
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: ";
    PrintTasksInfo(untouched_tasks);

    return 0;
}

