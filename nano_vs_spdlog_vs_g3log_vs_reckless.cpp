#include <chrono>
#include <thread>
#include <vector>
#include <cstdio>
#include <algorithm>
#include <cstring>
// #include "NanoLog.hpp"
// #include "spdlog/spdlog.h"
// #include "g3log/g3log.hpp"
// #include "g3log/logworker.hpp"
// #include "reckless/severity_log.hpp"
// #include "reckless/file_writer.hpp"

#include "NanoLogSystenZ.hpp"
// #include <system-z/utils/NanoLog.hpp>

/* Returns microseconds since epoch */
inline uint64_t timestamp_now()
{
    // return std::chrono::high_resolution_clock::now().time_since_epoch() / std::chrono::microseconds(1);
    timespec ts;
    ::clock_gettime(CLOCK_REALTIME, &ts);
    return (int64_t)ts.tv_sec * 1000000000LL + ts.tv_nsec;
}

template <typename Function>
void run_log_benchmark(Function &&f, char const *const logger)
{
    int iterations = 100000;
    std::vector<uint64_t> latencies;
    char const *const benchmark = "benchmark";
    for (int i = 0; i < iterations; ++i)
    {
        uint64_t begin = timestamp_now();
        f(i, benchmark);
        uint64_t end = timestamp_now();
        latencies.push_back(end - begin);
    }
    std::sort(latencies.begin(), latencies.end());
    uint64_t sum = 0;
    for (auto l : latencies)
    {
        sum += l;
    }
    //    printf("%s percentile latency numbers in nenoseconds\n%9s|%9s|%9s|%9s|%9s|%9s|%9s|\n%9ld|%9ld|%9ld|%9ld|%9ld|%9ld|%9lf|\n", logger, "50th", "75th", "90th", "99th", "99.9th", "Worst", "Average", latencies[(size_t)iterations * 0.5], latencies[(size_t)iterations * 0.75], latencies[(size_t)iterations * 0.9], latencies[(size_t)iterations * 0.99], latencies[(size_t)iterations * 0.999], latencies[latencies.size() - 1], (sum * 1.0) / latencies.size());
    printf("%s|%9ld|%9ld|%9ld|%9ld|%9ld|%9ld|%9lf\n", logger, latencies[(size_t)iterations * 0.5], latencies[(size_t)iterations * 0.75], latencies[(size_t)iterations * 0.9], latencies[(size_t)iterations * 0.99], latencies[(size_t)iterations * 0.999], latencies[latencies.size() - 1], (sum * 1.0) / latencies.size());
}

template <typename Function>
void run_benchmark(Function &&f, int thread_count, char const *const logger)
{
    printf("\nThread count: %d, latency numbers in nenoseconds\n", thread_count);
    printf("|%9s|%9s|%9s|%9s|%9s|%9s|%9s\n", "50th", "75th", "90th", "99th", "99.9th", "Worst", "Average");

    std::vector<std::thread> threads;
    for (int i = 0; i < thread_count; ++i)
    {
        threads.emplace_back(run_log_benchmark<Function>, std::ref(f), logger);
    }
    for (int i = 0; i < thread_count; ++i)
    {
        threads[i].join();
    }
}

void print_usage()
{
    char const *const executable = "nano_vs_spdlog_vs_g3log_vs_reckless";
    printf("Usage \n1. %s nanolog\n2. %s nanolog-system-z\n3. %s spdlog\n4. %s g3log\n5. %s reckless\n", executable, executable, executable, executable, executable);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        print_usage();
        return 0;
    }

    // if (strcmp(argv[1], "nanolog") == 0)
    // {
    //     // Guaranteed nano log.
    //     nanolog::initialize(nanolog::GuaranteedLogger(), "/tmp/", "nanolog", 1);

    //     auto nanolog_benchmark = [](int i, char const *const cstr)
    //     { LOG_INFO << "Logging " << cstr << i << 0 << 'K' << -42.42; };
    //     for (auto threads : {1, 2, 3, 4})
    //         run_benchmark(nanolog_benchmark, threads, "nanolog_guaranteed");
    // }
    if (strcmp(argv[1], "nanolog-system-z") == 0)
    {
        // Guaranteed nano log.
        nanolog_systen_z::initialize(nanolog_systen_z::GuaranteedLogger(), "/home/wsuser/work/panther-legacy-cpp/NanoLog-systemZ/", "nanolog-system-z", 20, "", -1);

        auto nanolog_benchmark = [](int i, char const *const cstr)
        { LOG_INFO(timestamp_now()) << "Logging " << cstr << i << 0 << 'K' << -42.42; };
        for (auto threads : {1, 2, 3, 4})
            run_benchmark(nanolog_benchmark, threads, "nanolog-system-z_guaranteed");
    }

    /*
    else if (strcmp(argv[1], "spdlog") == 0)
    {
        spdlog::set_async_mode(1048576);
        auto spd_logger = spdlog::create<spdlog::sinks::simple_file_sink_mt>("file_logger", "/tmp/spd-async.txt", false);

        auto spdlog_benchmark = [&spd_logger](int i, char const *const cstr)
        { spd_logger->info("Logging {}{}{}{}{}", cstr, i, 0, 'K', -42.42); };
        for (auto threads : {1, 2, 3, 4})
            run_benchmark(spdlog_benchmark, threads, "spdlog");
    }
    else if (strcmp(argv[1], "g3log") == 0)
    {
        auto worker = g3::LogWorker::createLogWorker();
        auto handle = worker->addDefaultLogger("g3", "/tmp/");
        g3::initializeLogging(worker.get());

        auto g3log_benchmark = [](int i, char const *const cstr)
        { LOGF(INFO, "Logging %s%d%d%c%lf", cstr, i, 0, 'K', -42.42); };
        for (auto threads : {1, 2, 3, 4})
            run_benchmark(g3log_benchmark, threads, "g3log");
    }
    else if (strcmp(argv[1], "reckless") == 0)
    {
        using log_t = reckless::severity_log<reckless::indent<4>, ' ', reckless::severity_field, reckless::timestamp_field>;
        reckless::file_writer writer("/tmp/reckless.txt");
        log_t g_log(&writer);

        auto reckless_benchmark = [&g_log](int i, char const *const cstr)
        { g_log.info("Logging %s%d%d%c%lf", cstr, i, 0, 'K', -42.42); };
        for (auto threads : {1, 2, 3, 4})
            run_benchmark(reckless_benchmark, threads, "reckless");
    }
    */
    else
    {
        print_usage();
    }

    return 0;
}
