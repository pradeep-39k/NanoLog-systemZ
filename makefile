all:
#	g++ -O0 -std=c++17 -w -Wall -pthread NanoLog.cpp non_guaranteed_nanolog_benchmark.cpp -o non_guaranteed_nanolog_benchmark
#	g++ -O0 -std=c++17 -w -Wall -o nano_vs_spdlog_vs_g3log_vs_reckless nano_vs_spdlog_vs_g3log_vs_reckless.cpp NanoLog.cpp -L/usr/local/lib -lpthread
#	g++ -O0 -std=c++17 -w -Wall -o nano_vs_spdlog_vs_g3log_vs_reckless nano_vs_spdlog_vs_g3log_vs_reckless.cpp  -L/usr/local/lib -lpthread -lsystem-z -lnuma
	g++  -O3  -std=c++17 -w -Wall -o nano_vs_spdlog_vs_g3log_vs_reckless nano_vs_spdlog_vs_g3log_vs_reckless.cpp NanoLogSystenZ.cpp -L/usr/local/lib -lpthread

#	g++ -g -O3 -std=c++11 -pthread NanoLog.cpp nano_vs_spdlog_vs_g3log_vs_reckless.cpp -I /home/karthik/spdlog/spdlog/include -I /home/karthik/g3log-master/src -L. -lg3logger -I /home/karthik/reckless/reckless/include -I /home/karthik/reckless/boost -L/home/karthik/reckless/reckless/lib -lasynclog -o nano_vs_spdlog_vs_g3log_vs_reckless
