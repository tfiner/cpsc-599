#if !defined(JSON_HELPERS_H_)
#define JSON_HELPERS_H_

#include <json/elements.h>
#include <json/writer.h>

#include <typeinfo>
#include <string>
#include <iostream>
#include <exception>


#define DECLARE_JSON_SERIALIZE(CLASS_NAME) \
	friend void load(CLASS_NAME&, const json::Object& jsonObj); \
	friend void save(const CLASS_NAME&, json::Object& jsonObj);

// #define JSON_DEBUG

namespace json {

	namespace cpp_serialize {

		typedef std::map< std::string, std::string > TypeNameMap;
		static TypeNameMap typeNameMap;

		template<typename Type>
		inline void register_type_name(const char* NAME) {
			typeNameMap[std::string(typeid(Type).name())] = std::string(NAME);
		}

		template<typename Type>
		inline std::string get_name(){
			const char* CLASS_NAME = typeid(Type).name();
			TypeNameMap::const_iterator iter = typeNameMap.find(CLASS_NAME);
			if ( iter == typeNameMap.end() ) {
				std::string msg("You forgot to call register_type_name<");
				msg += CLASS_NAME;
				msg += ">('short name')";
				throw std::logic_error( msg );
			}
			return iter->second;
		}


		template<typename Type>
		inline bool is_present(const json::Object& obj) {
			std::string type_name = get_name<Type>();
			json::Object::const_iterator iter = obj.Find(type_name);
			return iter != obj.End();
		}

		inline bool is_present(const json::Object& obj, const char* NAME) {
			return obj.Find(NAME) != obj.End();
		}


		inline std::string get_class_name(std::string parent, std::string child){
			return parent + "/" + child;
		}


		template<typename Parent, typename Child>
		Parent* load_inherit(const json::Object& obj) {
			const std::string KEY = get_class_name(get_name<Parent>(), get_name<Child>());
		#if defined(JSON_DEBUG)
			std::cout 	<< "FINDING KEY  " << KEY << "\n"
						<< "Members:\n";

			for ( json::Object::Members::const_iterator iter = obj.Begin(); iter != obj.End(); ++iter ) {
				std::cout << "   " << iter->name << "\n";
			}
			std::cout << std::endl;
		#endif
			json::Object::Members::const_iterator iter = obj.Find(KEY);
			if ( iter == obj.End() ) {
		#if defined(JSON_DEBUG)
				std::cout 	<< "MISS  " << KEY << std::endl;
		#endif
				return 0;
			}

			const json::Object& inner = (const json::Object&)iter->element;
		#if defined(JSON_DEBUG)
			std::cout << "HIT  Inner class:\n";
			json::Writer::Write(obj, std::cout);
			std::cout << std::endl;
		#endif
			Child* child = new Child;
			load(*child, inner);
			return child;
		}


		template<typename Parent, typename Child>
		bool save_inherit(const Parent& parentRef, json::Object& obj) {
			const Parent* parent = &parentRef;
			const Child* child = dynamic_cast<const Child*>(parent);
			if (child) {
				const std::string KEY = get_class_name(get_name<Parent>(), get_name<Child>());
				obj[KEY] = json::String(KEY.c_str());
				save(*child, obj[KEY]);
			}

			return child != 0;
		}

	} // cpp_serialize

} // json


#endif // JSON_HELPERS_H_

