#pragma once

#include <vector>
#include <string>


namespace stages {


auto preprocess_source(
  const std::string& src,
  const std::vector<std::string>& include_paths,
  std::string& output,
  bool verbose
) -> bool ;


auto link_objects(
  const std::vector<std::string>& obj_files,
  const std::vector<std::string>& lib_paths,
  const std::vector<std::string>& libraries,
  const std::string& output_file,
  bool verbose
) -> bool ;


}



