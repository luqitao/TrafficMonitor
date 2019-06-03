#include "JsonDocument.h"
#include <sstream>

namespace Huobi {

    JsonDocument::JsonDocument() {
    }

    JsonDocument::~JsonDocument() {
    }

    JsonWrapper JsonDocument::parseFromString(const std::string& text) {
        return parseFromString(text.c_str());
    }

    JsonWrapper JsonDocument::parseFromString(const char* text) {
        const rapidjson::Value& object = doc.Parse<rapidjson::kParseNumbersAsStringsFlag>(text);
        return JsonWrapper(object);
    }
	std::string JsonDocument::GetLastError() {
		std::ostringstream oss;
		if (doc.HasParseError()) // 通过HasParseError()来判断解析是否成功
		{
			oss << "parse error: " << doc.GetErrorOffset() << rapidjson::GetParseErrorFunc(doc.GetParseError());
		}
		return oss.str();
	}
}


