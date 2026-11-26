#include "httplib/httplib.h"

#include "jsonparser/json_object.h"
#include "jsonparser/json_array.h"

#include "escalonator/esc_abstract.h"
#include "escalonator/esc_fcfs.h"
#include "escalonator/esc_pcp.h"
#include "escalonator/esc_psp.h"
#include "escalonator/esc_rrsp.h"
#include "escalonator/esc_rrcp.h"
#include "escalonator/esc_sjf.h"
#include "escalonator/esc_srtf.h"
#include "escalonator/InputReader.h"

#include "allocator/memory.h"

int main() {
    httplib::Server svr;

    // TODO: Colocar esses endpoints dentro de alguma(s) classe(s)
    svr.Put("/escalonator/config", [](const httplib::Request &req, httplib::Response &res) {
            string config = req.body;
            ofstream configFile("config.txt");
            configFile << config;
            configFile.close();

            // TODO: Fazer uma checagem de validez do arquivo de configuração
            // antes de aceitá-lo como válido
            res.status = 201;
            res.set_content("Configurações definidas", "text/plain");
    });

    svr.Post("/escalonator/procs", [](const httplib::Request &req, httplib::Response &res) {
            string procsDesc = req.body;
            InputReader ir;

            Config config = ir.readConfig();
            std::vector<Process> processes = ir.readProcesses(procsDesc);

            esc_fcfs fcfs;
            esc_pcp pcp;
            esc_psp psp;
            esc_rrsp rrsp(config.quantum);
            esc_rrcp rrcp(config.quantum, config.aging);
            esc_sjf sjf;
            esc_srtf srtf;

            vector<Saida> saidas;

            saidas.push_back(fcfs.exec_process(processes));
            saidas.push_back(pcp.exec_process(processes));
            saidas.push_back(psp.exec_process(processes));
            saidas.push_back(rrsp.exec_process(processes));
            saidas.push_back(rrcp.exec_process(processes));
            saidas.push_back(sjf.exec_process(processes));
            saidas.push_back(srtf.exec_process(processes));

            json_array methods;
            for (int i = 0; i < saidas.size(); i++) {
                json_object method;

                Saida saida = saidas[i];

                json_array intervals;

                for (int j = 0; j < saidas[i].intervalos.size(); j++) {
                    json_object proc;
                    auto[iid, iini, ifim] = saidas[i].intervalos[j];

                    proc.add("\"id\"", iid);
                    proc.add("\"ini\"", iini);
                    proc.add("\"fim\"", ifim);

                    intervals.add(proc.text());
                }

                method.add("\"intervalos\"", intervals.text());
                method.add("\"id\"", "\"" + saida.id + "\"");
                method.add("\"tt\"", saida.tt);
                method.add("\"tw\"", saida.tw);
                method.add("\"num_trocas\"", saida.num_trocas);

                methods.add(method.text());
            }

            res.status = 201;
            res.set_content(methods.text(), "application/json");
    });

    // TODO: Implementar os 2 endpoints abaixo
    svr.Put("/allocator/cmds", [](const httplib::Request &req, httplib::Response &res) {
    });

    svr.Get("/allocator/states", [](const httplib::Request &req, httplib::Response &res) {
    });

    // TODO: Quem adicionou essas coisas?
    // svr.set_post_routing_handler([](const httplib::Request& req, httplib::Response& res) {
    //     res.set_header("Access-Control-Allow-Origin", "*");
    //     res.set_header("Access-Control-Allow-Methods", "POST, PUT, GET, OPTIONS");
    //     res.set_header("Access-Control-Allow-Headers", "Content-Type");
    // });

    // svr.Options(R"(.*)", [](const auto& req, auto& res) {
    //     res.status = 204;
    // });

    if (!svr.bind_to_port("0.0.0.0", 8080)) {
        std::cerr << "Falha ao tentar abrir uma conexão na porta 8080" << std::endl;
    }

    std::cout << "Servidor ouvindo na porta 8080" << std::endl;

    svr.listen_after_bind();

    return 0;
}
