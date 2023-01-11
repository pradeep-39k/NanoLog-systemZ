#include <system-z/exe/json-poller/Parameter.hpp>

namespace wallsoft{


void Parameter::initialize(Json::Value value){

    if (!value.empty() && value.isObject()
        && value.isMember("i") && value["i"].isInt()
        && value.isMember("l") && value["l"].isIntegral()
        && value.isMember("b") && value["b"].isBool()
        && value.isMember("s") && value["s"].isString()
        && value.isMember("v") && value["v"].isArray()){

		this->i = value["i"].asInt();
		this->l = value["l"].asInt64();
		this->b = value["b"].asBool();
		this->s = value["s"].asString();

        for(int j; j < value["v"].size(); j++){

            if(!value["v"][j].isInt())
                throw std::string("Error: Parameter::initialize(): invalid value in array. Invalid json. ") + value.toStyledString();

            v.push_back(value["v"].asInt());
        }

	}
	else
		throw std::string("Error: Parameter::setJsonValue(): Invalid json. ") + value.toStyledString();	

}



Json::Value Parameter::getJsonValue(){

    Json::Value value;

    value["i"] = this->i;
	value["l"] = (Json::Int64) this->l;
    value["b"] = this->b;
    value["s"] = this->s;

    Json::Value array(Json::arrayValue);
	for(int j = 0; j < this->v.size(); j++)
		array[j] = this->v[j];

	value["v"] = array;

	return value;

}


}