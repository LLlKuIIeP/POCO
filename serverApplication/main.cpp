#include <iostream>
#include <fstream>

#include <Poco/Util/ServerApplication.h>
#include <Poco/Task.h>
#include <Poco/TaskManager.h>
#include <Poco/Exception.h>

class myTask: public Poco::Task
{
public:
    myTask(): Task("myTask") //регистрируем задачу под имененм myTask
    {
    }

    //запуск
    void runTask()
    {
        Poco::Util::Application &app = Poco::Util::Application::instance();


        while(!isCancelled()) {
            std::ofstream out("/home/user/log.log", std::ios_base::trunc);
            out << "task work\n";
            out.flush();
        }
    }

    ~myTask()
    {
    }
};


class myProg: public Poco::Util::ServerApplication
{
protected:
    void initialize(Poco::Util::Application &app)
    {
        Poco::Util::ServerApplication::initialize(app);

        //logger().setChannel("log.log", );
    }

    void uninitialize()
    {
        Poco::Util::ServerApplication::uninitialize();
    }

    int main(const std::vector<std::string> &args)
    {
        //если запустился как не демон и не сервис
        if(config().getBool("application.runAsDeamon", false) && config().getBool("application.runAsService", false)) {
            std::ofstream out("progLog.log");
            out << "Error\n";
            out.flush();
            out.close();
        }
        else {
            //запустился как сервис или демон
            std::ofstream out("progLog.log");
            out << "Success\n";
            out.flush();
            out.close();

            //создаем задачу
            Poco::TaskManager taskManager;

            //запускаем
            taskManager.start(new myTask);

            //ждем завершения
            waitForTerminationRequest();

            //завершаем все задачи и потоки
            taskManager.cancelAll();
            taskManager.joinAll();
        }

        return Poco::Util::Application::EXIT_OK;
    }


};


POCO_SERVER_MAIN(myProg)
//int main(int argc, char** argv)
//{
//    myProg prog;
//    return prog.run(argc, argv);
//    //return 0;
//}
