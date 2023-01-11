#include <iostream>

#include <system-z/utils/Time.hpp>
#include <system-z/utils/ThreadUtils.hpp>
#include <system-z/utils/JsonPoller.hpp>

#include <system-z/exe/json-poller/Parameter.hpp>

using namespace wallsoft;


int main(int argc, char* argv[]){

    if(argc != 2){
        std::cout << "Usage:\n\tjson-poller-test <jsonFilePath>\n"; 
        return -1;
    }

    try{

        ThreadUtils::setThreadName("json-poller-test");
        ThreadUtils::setAffinityAndMoveStack(1);

        std::string jsonFilePath(argv[1]);

        JsonPoller<Parameter> jsonPoller(jsonFilePath, 1, "json-poller", 2);

        Parameter parameter;

        while(true){

            // Critical thread.
            // Do some real work here. 
            // BUGBUG: Should not be 'sleep' in real life usage.
            std::this_thread::sleep_for(std::chrono::milliseconds(300));


            // Maintenance in Critical thread.
            // Check for parameter.
            int64_t t1 = Time::getSystemTimestamp();
            if(jsonPoller.pop(parameter)){
             
                int64_t t2 = Time::getSystemTimestamp();
                std::cout << "\nParameter poped. duration:" << t2 - t1 << "\n" << parameter.getJsonValue().toStyledString(); 
            }
        
        }

    }
    catch(std::string s){
        std::cout << "\nException:\n" << s << "\n";
    } 
    

    return 0;

}