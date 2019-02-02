#ifndef ESSConfigManager_h
#define ESSConfigManager_h 1

#include <string>

class ESSConfigManager {
public:
  ESSConfigManager(std::string config_filename);
  virtual ~ESSConfigManager();

protected:
  void parseConfig();

private:
  std::string config_filename;
};

#endif