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

#include "pager/pager.h"
#include "pager/pagerFIFO.h"

int main() {
    httplib::Server svr;

    // TODO: Colocar esses endpoints dentro de alguma(s) classe(s)
    svr.Put("/escalonator/config", [](const httplib::Request &req, httplib::Response &res) {
	string config = req.body;
	ofstream configFile("escalonator-config.txt");
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

    svr.Post("/allocator/cmds", [](const httplib::Request &req, httplib::Response &res) {
        std::stringstream cmds;
        cmds << req.body;
        std::string cmd;

        memory mem;

        json_array info;

        while (cmds >> cmd) {
            if (cmd == "show" || cmd == "stats") continue;
            if (cmd == "init") {
                int sz;
                cmds >> sz;
                mem.init(sz);
            } else if (cmd == "alloc") {
                int sz;
                string alloc_mode_str;
                alloc_mode am;
                cmds >> sz;
                cmds >> alloc_mode_str;
                if (alloc_mode_str == "first") {
                    am = FIRST;
                } else if (alloc_mode_str == "best") {
                    am = BEST;
                } else if (alloc_mode_str == "worst") {
                    am = WORST;
                } else if (alloc_mode_str == "next") {
                    am = NEXT;
                }
                mem.alloc(sz, am);
            } else if (cmd == "freeid") {
                int id;
                cmds >> id;
                mem.freeid(id);
            } else if (cmd == "freeaddr") {
                int addr;
                cmds >> addr;
                mem.freeaddr(addr);
            } else if (cmd == "defrag") {
                string defrag_mode_str;
                defrag_mode dm;

                cmds >> defrag_mode_str;
                if (defrag_mode_str == "init") {
                    dm = INIT;
                } else if (defrag_mode_str == "end") {
                    dm = END;
                } else if (defrag_mode_str == "opt") {
                    dm = OPT;
                }
                mem.defrag(dm);
            }

            // Exibição da disposição e estatísticas atuais da memória
            json_object curr_info;
            json_array curr_blocos;

            for (auto b : mem.blocos) {
                json_object jbloco;
                jbloco.add("\"id\"", b.id);
                jbloco.add("\"ini\"", b.ini);
                jbloco.add("\"sz\"", b.sz);
                curr_blocos.add(jbloco.text());
            }

            curr_info.add("\"blocos\"", curr_blocos.text());
            curr_info.add("\"ocupado\"", mem.ocuppation);
            curr_info.add("\"livre\"", mem.memory_sz - mem.ocuppation);
            curr_info.add("\"buracos\"", mem.cnt_holes());
            curr_info.add("\"maior buraco\"", mem.l_hole());
            curr_info.add("\"uso efetivo\"", (float) mem.ocuppation / mem.memory_sz);

            info.add(curr_info.text());
        }

        res.status = 201;
        res.set_content(info.text(), "application/json");
    });

    svr.Put("/pager/config", [](const httplib::Request &req, httplib::Response &res) {
	string config = req.body;
	ofstream configFile("pager-config.txt");
	configFile << config;
	configFile.close();

	// TODO: Fazer uma checagem de validez do arquivo de configuração
	// antes de aceitá-lo como válido
	res.status = 201;
	res.set_content("Configurações definidas", "text/plain");
    });

    svr.Post("/pager/pages", [](const httplib::Request &req, httplib::Response &res) {
	if (!std::filesystem::exists("./pager-config.txt")) {
	    std::cerr << "Erro: Crie um arquivo de texto nomeado \"pager-config.txt\" com as informações sobre o número de frames e a política de substituição a serem utilizados." << std::endl;
	    std::exit(EXIT_FAILURE);
	}

	std::ifstream configFile("./pager-config.txt");

	int sz;
	string pag_policy_str;
	string line;

	while (configFile >> line) {
	    int twoDotsPos = line.find(":");
	    if (twoDotsPos == line.size()) {
		std::cout << "Erro: Arquivo de configuração fora do padrão." << std::endl;
		std::exit(EXIT_FAILURE);
	    }
	    std::string key = line.substr(0, twoDotsPos);
	    // TODO: Verificar se esse valor é realmente um número inteiro antes de tentar convertê-lo
	    std::string value = line.substr(twoDotsPos+1, line.size() - twoDotsPos - 1);
	    if (key == "algo") pag_policy_str = value;
	    else if (key == "frames") sz = stoi(value);
	    else {
		std::cout << "Erro: Opção inválida passada no arquivo de configuração." << std::endl;
		std::exit(EXIT_FAILURE);
	    }
	}

        std::stringstream accs;
        accs << req.body;
	    
	vector<acc> accseq;

	while (std::getline(accs, line)) {
	    if (line[0] == '#') continue; // Linha de comentário
	    acc a;
	    if (line[line.size()-1] == 'W') {
		a.at = WRITE;
	    } else {
		a.at = READ;
	    }
	    a.page_id = stoi(line.substr(0, line.find(" ")));
	    accseq.push_back(a);
	}

	saida_pag sp;

	PagerFIFO pagfifo(sz, accseq);
	// PagerLRU pagfifo(sz, accseq);
	// PagerOPTIMAL pagfifo(sz, accseq);
	// PagerCLOCK pagfifo(sz, accseq);
	// PagerNRU pagfifo(sz, accseq);
	// PagerLFU pagfifo(sz, accseq);
	// PagerMFU pagfifo(sz, accseq);

	if (pag_policy_str == "fifo") sp = pagfifo.access();
	// else if (pag_policy_str == "lru") pp = LRU;
	// else if (pag_policy_str == "optimal") pp = OPTIMAL;
	// else if (pag_policy_str == "clock") pp = CLOCK;
	// else if (pag_policy_str == "nru") pp = NRU;
	// else if (pag_policy_str == "lfu") pp = LFU;
	// else if (pag_policy_str == "mfu") pp = MFU;

	json_array json_horms;

	// TODO: Fazer uma checagem de validez do arquivo de configuração
	// antes de aceitá-lo como válido
	res.status = 201;
	res.set_content(json_horms.text(), "application/json");
    });

    svr.set_post_routing_handler([](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, PUT, GET, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
    });

    svr.Options(R"(.*)", [](const auto& req, auto& res) {
        res.status = 204;
    });

    if (!svr.bind_to_port("0.0.0.0", 8080)) {
        std::cerr << "Falha ao tentar abrir uma conexão na porta 8080" << std::endl;
    }

    std::cout << "Servidor ouvindo na porta 8080" << std::endl;

    svr.listen_after_bind();

    return 0;
}
