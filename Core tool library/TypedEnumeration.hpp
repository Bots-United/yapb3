//
// Copyright © 2003-2010, by YaPB Development Team. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// Definitions.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CORE_TOOL_LIBRARY_DEFINITIONS_INCLUDED
#define CORE_TOOL_LIBRARY_DEFINITIONS_INCLUDED

#define DEFINE_SIMPLE_ENUM(EnumName, seq) \
struct EnumName	\
{	\
	enum type	\
	{	\
		BOOST_PP_SEQ_FOR_EACH_I(DEFINE_SIMPLE_ENUM_VAL, EnumName, seq)	\
	};	\
	type v;	\
	inline EnumName (type v) : v (v) { /* VOID */ }	\
	inline operator type (void) const { return v; }	\
	\
private:	\
	/* Предотвращает неявное преобразование значений перечисления
	* к любым типам, кроме типа type, что препятствует сравнению
	* значений перечислений с интегральными типами или со значениями
	* других перечислений
	*/	\
	template <typename T> inline operator T (void) const;	\
};	\

#define DEFINE_SIMPLE_ENUM_VAL(r, data, i, record) \
	BOOST_PP_TUPLE_ELEM(2, 0, record) = BOOST_PP_TUPLE_ELEM(2, 1, record),

namespace detail
{
	/** Пустой тэг для связывания со значением enum'а */
	struct EmptyTag
	{
	};
}

/** Для задания признака то, является перечисление строгим
* (при этом генерируется исключение при попытке установить
* значение перечислению отличное от заданного) или нет
*/
enum Strictness
{
	enum_strict,	//!< перечисление является строгим
	enum_nonstrict	//!< перечисление не является строгим (по-умолчанию)
};

/** Базовый класс для типобезопасных перечислений
*
* Пример использования смотри в конце файла
*/
template<typename Derived, typename Type = int,
Strictness strict = enum_nonstrict,
typename TagType = detail::EmptyTag>
class StrictEnumBase
{
	/** Класс с описанием значения перечисления */
	class EntryDescription
	{
	public:
		/** Конструктор */
		inline EntryDescription (Type value, const DynamicString &internalName, const DynamicString &externalName) :
			value_ (value),
			internalName_ (internalName),
			externalName_ (externalName)
		{ /* VOID */ }

		/** Значение */
		inline const Type Value (void) const
		{
			return value_;
		}

		/** Внутреннее имя */
		inline const DynamicString InternalName (void) const
		{
			return internalName_;
		}

		/** Внешнее имя */
		inline const DynamicString ExternalName (void) const
		{
			return externalName_;
		}

	private:
		/*const*/ Type value_;			//!< значение
		/*const*/ DynamicString internalName_;	//!< внутреннее имя
		/*const*/ DynamicString externalName_;	//!< внешнее имя
	};

	/** Информация, сопоставляемая с каждым элементом - описание и пользовательский тэг */
	class EntryInfo
	{
	public:
		/** Конструктор
		*/
		inline EntryInfo (const EntryDescription &desc, const TagType &tag) : desc_ (desc), tag_ (tag) { /* VOID */ }

		/** Получить описание
		*/
		inline const EntryDescription &Description (void) const
		{
			return desc_;
		}

		/** Получить тэг
		*/
		inline const TagType &Tag (void) const
		{
			return tag_;
		}

	private:
		/*const*/ EntryDescription desc_;	//!< описание
		/*const*/ TagType tag_;				//!< тэг
	};


	/** Внутренний тип для хранения информации об элементах */
	typedef std::map <Type, EntryInfo> IntEntries;

public:	// Comparison operators
	inline const bool operator < (const StrictEnumBase &other) const
	{
		return value_ < other.value_;
	}

	inline const bool operator > (const StrictEnumBase &other) const
	{
		return value_ > other.value_;
	}

	inline const bool operator <= (const StrictEnumBase &other) const
	{
		return value_ <= other.value_;
	}

	inline const bool operator >= (const StrictEnumBase &other) const
	{
		return value_ >= other.value_;
	}

	inline const bool operator == (const StrictEnumBase &other) const
	{
		return value_ == other.value_;
	}

	inline const bool operator != (const StrictEnumBase &other) const
	{
		return value_ != other.value_;
	}
public:	// Public interface
	/** Получить значение перечисления */
	inline const Type &Value (void) const
	{
		return value_;
	}

	/** Получить описание значения.
	* Можно получать и для неизвестных значений
	*/
	inline const EntryDescription Description (void) const
	{
		IntEntries::iterator iter = GetIntEntries ().find (value_);
		if (iter == GetIntEntries ().end ())
			return MakeEmptyDescription (value_);
		return iter->second.Description ();
	}

	/** Получить тэг, связанный со значением
	* Можно получать только для известных значений
	* @return Тэг
	* @throw std::out_of_range Если вызывается для неизвестного значения
	*/
	inline const TagType &Tag (void) const// throw (std::out_of_range)
	{
		IntEntries::iterator iter = GetIntEntries ().find (value_);
		if (iter == GetIntEntries ().end ())
			throw std::out_of_range ("StrictEnum: out of range");
		return iter->second.Tag ();
	}

	/** Внутреннее имя значения перечисления */
	inline const DynamicString InternalName (void) const
	{
		return Description ().InternalName ();
	}

	/** Внешнее имя значения перечисления */
	inline const DynamicString ExternalName (void) const
	{
		return Description ().ExternalName ();
	}

	/** Проверить, является ли значение известным */
	inline const bool IsKnown (void) const
	{
		return IsExist (value_);
	}
public:	// Static functions
	/** Тип для хранения информации об элементах */
	typedef std::vector <EntryDescription> Entries;

	/** Проверка существования заданного значения в перечислении */
	static inline const bool IsExist (Type value)
	{
		return GetIntEntries ().find (value) != GetIntEntries ().end ();
	}

	/** Создать объект из значения
	* @throw std::out_of_range Если устанавливается неправильное
	* значение (только в случае strict == enum_strict).
	*/
	static inline const Derived FromValue (Type value)
	{
		AssertExist (value);
		return Derived (value);
	}

	/** Создать объект из значения. Если значение неправильное,
	* то возвращает значение по-умолчанию
	* @param defaultValue Значение по-умолчанию
	*/
	static inline const Derived FromValue (Type value, Derived defaultValue)
	{
		if (IsExist (value))
			return Derived (value);
		else
			return defaultValue;
	}

	/** Получить описания всех известных значений перечисления */
	static inline const Entries GetEntries (void)
	{
		// Копируем описания элементов из внутреннего
		// представления во внешнее
		IntEntries intEntries = GetIntEntries ();
		Entries entries;
		entries.reserve (intEntries.size ());
		for (IntEntries::const_iterator iter = intEntries.begin ();
			iter != intEntries.end (); ++iter)
			entries.push_back (iter->second.Description ());
		return entries;
	}

protected:
	/** Тип значения */
	typedef Type ValueType;

	/** Конструктор для известных значений */
	inline StrictEnumBase (Type value, const DynamicString &internalName, const DynamicString &externalName, const TagType &tag = TagType ()) :
		value_ (value),
		valueName_ (internalName)
	{
		GetIntEntries ().insert (std::make_pair (value, EntryInfo (EntryDescription (value, internalName, externalName), tag)));
	}

	/** Конструктор для неизвестных значений */
	explicit inline StrictEnumBase (Type value) : value_ (value) { /* VOID */ }

private:
	/** Непосредственно значение */
	Type value_;
	/**
	* Строковое представление значения (нужно только для визуализации, в противном случае можно удалить)
	*/
	DynamicString valueName_;

	/** Получить множество значений, которые могут содержаться
	* в данном перечислении
	*/
	static inline IntEntries &GetIntEntries (void)
	{
		static IntEntries entries;

		return entries;
	}

	/** Проверить, что значение является известным
	* @throw std::out_of_range Если неизвестное значение
	*/
	static inline void AssertExist (Type value)
	{
		if (strict == enum_strict && !IsExist (value))
			throw std::out_of_range ("StrictEnum: out of range");
	}

	/** Создать описание для неизвестного значения
	*/
	static inline const EntryDescription MakeEmptyDescription (Type value)
	{
		std::ostringstream stream;
		stream << "<" << value << ">";
		return EntryDescription (value, stream.str (), "");
	}
};

#define DEFINE_STRICT_ENUM (EnumName, Type, seq) \
class EnumName : public tools::common::StrictEnumBase<EnumName, Type> { \
	friend class tools::common::StrictEnumBase<EnumName, Type>; \
	EnumName (ValueType value, const DynamicString &sIntName, const DynamicString &sExtName) \
	: tools::common::StrictEnumBase<EnumName, Type> (value, sIntName, sExtName) \
		{} \
		EnumName (ValueType value) \
		: tools::common::StrictEnumBase<EnumName, Type> (value) \
		{} \
public: \
	enum { \
	BOOST_PP_SEQ_FOR_EACH_I (DEFINE_STRICT_ENUM_VAL, EnumName, seq) \
	}; \
	BOOST_PP_SEQ_FOR_EACH_I (DEFINE_STRICT_ENUM_DEF, EnumName, seq);\
};\
	BOOST_PP_SEQ_FOR_EACH_I (DEFINE_STRICT_ENUM_DECL, EnumName, seq);

#define DEFINE_STRICT_ENUM_DEF (r, aux, i, record) \
	static const aux BOOST_PP_TUPLE_ELEM (2, 0, record);

#define DEFINE_STRICT_ENUM_VAL (r, aux, i, record) \
	BOOST_PP_TUPLE_ELEM (2, 0, record)_ = BOOST_PP_TUPLE_ELEM (2, 1, record),


#define DEFINE_STRICT_ENUM_DECL (r, aux, i, record) \
	__declspec (selectany) const aux aux::BOOST_PP_TUPLE_ELEM (2, 0, record) (BOOST_PP_TUPLE_ELEM (2, 1, record), BOOST_PP_STRINGIZE (BOOST_PP_TUPLE_ELEM (2, 0, record)), "");

#define DEFINE_STRICT_ENUM_TAG (EnumName, Type, strict_f, tag_t, seq) \
class EnumName : public tools::common::StrictEnumBase<EnumName, Type, strict_f, tag_t> { \
	friend class tools::common::StrictEnumBase<EnumName, Type, strict_f, tag_t>; \
	EnumName (ValueType value, const DynamicString &sIntName, const DynamicString &sExtName, tag_t tag) \
	: tools::common::StrictEnumBase<EnumName, Type, strict_f, tag_t> (value, sIntName, sExtName, tag) \
		{} \
		EnumName (ValueType value) \
		: tools::common::StrictEnumBase<EnumName, Type, strict_f, tag_t> (value) \
		{} \
public: \
	BOOST_PP_SEQ_FOR_EACH_I (DEFINE_STRICT_ENUM_TAG_DEF, EnumName, seq);\
};\
	BOOST_PP_SEQ_FOR_EACH_I (DEFINE_STRICT_ENUM_TAG_DECL, EnumName, seq);

#define DEFINE_STRICT_ENUM_TAG_DEF (r, aux, i, record) \
	static const aux BOOST_PP_TUPLE_ELEM (3, 0, record);

#define DEFINE_STRICT_ENUM_TAG_DECL (r, aux, i, record) \
	__declspec (selectany) const aux aux::BOOST_PP_TUPLE_ELEM (3, 0, record) (BOOST_PP_TUPLE_ELEM (3, 1, record), BOOST_PP_STRINGIZE (BOOST_PP_TUPLE_ELEM (3, 0, record)), "", BOOST_PP_TUPLE_ELEM (3, 2, record));



#define DEFINE_STRICT_ENUM_WITH_DESC (EnumName, Type, seq) \
class EnumName : public tools::common::StrictEnumBase<EnumName, Type> { \
	friend class tools::common::StrictEnumBase<EnumName, Type>; \
	EnumName (ValueType value, const DynamicString &internalName, const DynamicString &externalName) \
	: tools::common::StrictEnumBase<EnumName, Type> (value, internalName, externalName) \
		{} \
	EnumName (ValueType value) \
		: tools::common::StrictEnumBase<EnumName, Type> (value) \
	{} \
public: \
	BOOST_PP_SEQ_FOR_EACH_I (DEFINE_STRICT_ENUM_WITH_DESC_DEF, EnumName, seq);\
};\
	BOOST_PP_SEQ_FOR_EACH_I (DEFINE_STRICT_ENUM_WITH_DESC_DECL, EnumName, seq);

#define DEFINE_STRICT_ENUM_WITH_DESC_DEF (r, aux, i, record) \
	static const aux BOOST_PP_TUPLE_ELEM (4, 0, record);

#define DEFINE_STRICT_ENUM_WITH_DESC_DECL (r, aux, i, record) \
	__declspec (selectany) const aux aux::BOOST_PP_TUPLE_ELEM (4, 0, record) (BOOST_PP_TUPLE_ELEM (4, 1, record), BOOST_PP_STRINGIZE (BOOST_PP_TUPLE_ELEM (4, 2, record)), BOOST_PP_TUPLE_ELEM (4, 3, record));

#define DEFINE_STRICT_ENUM_WITH_NAME (EnumName, Type, seq) \
class EnumName : public tools::common::StrictEnumBase<EnumName, Type> { \
	friend class tools::common::StrictEnumBase<EnumName, Type>; \
	EnumName (ValueType value, const DynamicString &internalName, const DynamicString &externalName) \
	: tools::common::StrictEnumBase<EnumName, Type> (value, internalName, externalName) \
		{} \
		EnumName (ValueType value) \
		: tools::common::StrictEnumBase<EnumName, Type> (value) \
	{} \
public: \
	BOOST_PP_SEQ_FOR_EACH_I (DEFINE_STRICT_ENUM_WITH_NAME_DEF, EnumName, seq);\
};\
	BOOST_PP_SEQ_FOR_EACH_I (DEFINE_STRICT_ENUM_WITH_NAME_DECL, EnumName, seq);

#define DEFINE_STRICT_ENUM_WITH_NAME_DEF (r, aux, i, record) \
	static const aux BOOST_PP_TUPLE_ELEM (3, 0, record);

#define DEFINE_STRICT_ENUM_WITH_NAME_DECL (r, aux, i, record) \
	__declspec (selectany) const aux aux::BOOST_PP_TUPLE_ELEM (3, 0, record) \
 (BOOST_PP_TUPLE_ELEM (3, 1, record), BOOST_PP_STRINGIZE (BOOST_PP_TUPLE_ELEM (3, 0, record)), "");

/*
Примеры использования строгих перечислений:

DEFINE_STRICT_ENUM (Color, int,
	((Red,      1))
	((Black,    3))
	((Green,    5))
)
DEFINE_STRICT_ENUM_TAG (Color, int, enum_strict, ColorCategory,
	((Red,      1, (ColorCategory::Bright) ))
	((Black,    3, (ColorCategory::Dark)   ))
	((Green,    5, (ColorCategory::Bright) ))
)
*/

#endif	// ifndef CORE_TOOL_LIBRARY_DEFINITIONS_INCLUDED