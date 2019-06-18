#pragma once

namespace Discord {
	class Message {
	public:
		std::string content;
		
		inline std::string getContent() { return content; }
	};
}