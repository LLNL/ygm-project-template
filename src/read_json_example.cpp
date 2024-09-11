
#include <string>

#include <ygm/comm.hpp>
#include <ygm/io/ndjson_parser.hpp>

int main(int argc, char **argv) {

  ygm::comm world(&argc, &argv);

  ygm::io::ndjson_parser jp(world,
                            std::vector<std::string>({"./data/json_files/"}));

  jp.for_all([&world](const auto &json_line) {
    world.cout("id: ", json_line.at("id"));
  });

  return 0;
}
