#pragma once

#include "myClient.hpp"
#include <thread>
#include <memory>

int startImguiClient(std::shared_ptr<MyClient> client, std::shared_ptr<std::thread> clientThread);