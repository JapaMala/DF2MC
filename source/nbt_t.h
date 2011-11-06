#pragma once

#include <stdint.h>

//Named Binary Tag specification
//
//NBT (Named Binary Tag) is a tag based binary format designed to carry large amounts of binary data with smaller amounts of additional data.
//An NBT file consists of a single GZIPped Named Tag of type TAG_Compound.
//
//A Named Tag has the following format:
//
//    byte tagType
//    TAG_String name
//    [payload]
//    
//The tagType is a single byte defining the contents of the payload of the tag.
//
//The name is a descriptive name, and can be anything (eg "cat", "banana", "Hello World!"). It has nothing to do with the tagType.
//The purpose for this name is to name tags so parsing is easier and can be made to only look for certain recognized tag names.
//Exception: If tagType is TAG_End, the name is skipped and assumed to be "".
//
//The [payload] varies by tagType.
//
//Note that ONLY Named Tags carry the name and tagType data. Explicitly identified Tags (such as TAG_String above) only contains the payload. 
//
//
//The tag types and respective payloads are:
//
//    TYPE: 0  NAME: TAG_End
//    Payload: None.
//    Note:    This tag is used to mark the end of a list.
//             Cannot be named! If type 0 appears where a Named Tag is expected, the name is assumed to be "".
//             (In other words, this Tag is always just a single 0 byte when named, and nothing in all other cases)
//    
//    TYPE: 1  NAME: TAG_Byte
//    Payload: A single signed byte (8 bits)
//
//    TYPE: 2  NAME: TAG_Short
//    Payload: A signed short (16 bits, big endian)
//
//    TYPE: 3  NAME: TAG_Int
//    Payload: A signed short (32 bits, big endian)
//
//    TYPE: 4  NAME: TAG_Long
//    Payload: A signed long (64 bits, big endian)
//
//    TYPE: 5  NAME: TAG_Float
//    Payload: A floating point value (32 bits, big endian, IEEE 754-2008, binary32)
//
//    TYPE: 6  NAME: TAG_Double
//    Payload: A floating point value (64 bits, big endian, IEEE 754-2008, binary64)
//    
//    TYPE: 7  NAME: TAG_Byte_Array
//    Payload: TAG_Int length 
//             An array of bytes of unspecified format. The length of this array is <length> bytes
//
//    TYPE: 8  NAME: TAG_String
//    Payload: TAG_Short length 
//             An array of bytes defining a string in UTF-8 format. The length of this array is <length> bytes
//
//    TYPE: 9  NAME: TAG_List
//    Payload: TAG_Byte tagId
//             TAG_Int length
//             A sequential list of Tags (not Named Tags), of type <typeId>. The length of this array is <length> Tags
//    Notes:   All tags share the same type.
//             
//    TYPE: 10 NAME: TAG_Compound
//    Payload: A sequential list of Named Tags. This array keeps going until a TAG_End is found.
//             TAG_End end
//    Notes:   If there's a nested TAG_Compound within this tag, that one will also have a TAG_End, so simply reading until the next TAG_End will not work.
//             The names of the named tags have to be unique within each TAG_Compound
//             The order of the tags is not guaranteed.
//             
//             
//
//
//
//Decoding example:
//(Use http://www.minecraft.net/docs/test.nbt to test your implementation)
//
//
//First we start by reading a Named Tag.
//After unzipping the stream, the first byte is a 10. That means the tag is a TAG_Compound (as expected by the specification).
//
//The next two bytes are 0 and 11, meaning the name string consists of 11 UTF-8 characters. In this case, they happen to be "hello world".
//That means our root tag is named "hello world". We can now move on to the payload.
//
//From the specification, we see that TAG_Compound consists of a series of Named Tags, so we read another byte to find the tagType.
//It happens to be an 8. The name is 4 letters long, and happens to be "name". Type 8 is TAG_String, meaning we read another two bytes to get the length,
//then read that many bytes to get the contents. In this case, it's "Bananrama".
//
//So now we know the TAG_Compound contains a TAG_String named "name" with the content "Bananrama"
//
//We move on to reading the next Named Tag, and get a 0. This is TAG_End, which always has an implied name of "". That means that the list of entries
//in the TAG_Compound is over, and indeed all of the NBT file.
//
//So we ended up with this:
//
//	TAG_Compound("hello world"): 1 entries
//	{
//	   TAG_String("name"): Bananrama
//	}
//
//
//
//For a slightly longer test, download http://www.minecraft.net/docs/bigtest.nbt
//You should end up with this:
//
//	TAG_Compound("Level"): 11 entries
//	{
//	   TAG_Short("shortTest"): 32767
//	   TAG_Long("longTest"): 9223372036854775807
//	   TAG_Float("floatTest"): 0.49823147
//	   TAG_String("stringTest"): HELLO WORLD THIS IS A TEST STRING ���!
//	   TAG_Int("intTest"): 2147483647
//	   TAG_Compound("nested compound test"): 2 entries
//	   {
//	      TAG_Compound("ham"): 2 entries
//	      {
//	         TAG_String("name"): Hampus
//	         TAG_Float("value"): 0.75
//	      }
//	      TAG_Compound("egg"): 2 entries
//	      {
//	         TAG_String("name"): Eggbert
//	         TAG_Float("value"): 0.5
//	      }
//	   }
//	   TAG_List("listTest (long)"): 5 entries of type TAG_Long
//	   {
//	      TAG_Long: 11
//	      TAG_Long: 12
//	      TAG_Long: 13
//	      TAG_Long: 14
//	      TAG_Long: 15
//	   }
//	   TAG_Byte("byteTest"): 127
//	   TAG_List("listTest (compound)"): 2 entries of type TAG_Compound
//	   {
//	      TAG_Compound: 2 entries
//	      {
//	         TAG_String("name"): Compound tag #0
//	         TAG_Long("created-on"): 1264099775885
//	      }
//	      TAG_Compound: 2 entries
//	      {
//	         TAG_String("name"): Compound tag #1
//	         TAG_Long("created-on"): 1264099775885
//	      }
//	   }
//	   TAG_Byte_Array("byteArrayTest (the first 1000 values of (n*n*255+n*7)%100, starting with n=0 (0, 62, 34, 16, 8, ...))"): [1000 bytes]
//	   TAG_Double("doubleTest"): 0.4931287132182315
//	}

//base tag class.

int is_big_endian(void);


//pre-declarations.
class TAG_Byte;
class TAG_Short;
class TAG_Int;
class TAG_Long;
class TAG_Float;
class TAG_Double;
class TAG_Byte_Array;
class TAG_String;
class TAG_List;
class TAG_Compound;


class TAG
{
    TAG_String * label;
public:
    virtual uint8_t get_type() = 0;
    virtual uint32_t get_size() = 0;
    virtual bool write_nbt_data(uint32_t  & cursor, uint8_t * data, uint32_t size) = 0;
    friend class TAG_Compound;
};

class TAG_Byte: public TAG
{
    uint8_t payload_byte;
public:
    uint8_t get_type(){return 1;}
    uint32_t get_size(){return 1;}
    bool write_nbt_data(uint32_t  & cursor, uint8_t * data, uint32_t size);
    friend class TAG_Byte_Array;
    friend class TAG_List;
};

class TAG_Short: public TAG
{
    union
    {
    int16_t payload_short;
    uint8_t byte_array[2];
    };
public:
    uint8_t get_type(){return 2;}
    uint32_t get_size(){return 2;}
    bool write_nbt_data(uint32_t  & cursor, uint8_t * data, uint32_t size);
    friend class TAG_String;
};

class TAG_Int: public TAG
{
    union
    {
    int32_t payload_int;
    uint8_t byte_array[4];
    };
public:
    uint8_t get_type(){return 3;}
    uint32_t get_size(){return 4;}
    bool write_nbt_data(uint32_t  & cursor, uint8_t * data, uint32_t size);
    friend class TAG_List;
    friend class TAG_Compound;
};

class TAG_Long: public TAG
{
    union
    {
    int64_t payload_long;
    uint8_t byte_array[8];
    };
public:
    uint8_t get_type(){return 4;}
    uint32_t get_size(){return 8;}
    bool write_nbt_data(uint32_t  & cursor, uint8_t * data, uint32_t size);
};

class TAG_Float: public TAG
{
    union
    {
    float payload_float;
    uint8_t byte_array[4];
    };
public:
    uint8_t get_type(){return 5;}
    uint32_t get_size(){return 4;}
    bool write_nbt_data(uint32_t  & cursor, uint8_t * data, uint32_t size);
};

class TAG_Double: public TAG
{
    union
    {
    double payload_float;
    uint8_t byte_array[8];
    };
public:
    uint8_t get_type(){return 6;}
    uint32_t get_size(){return 8;}
    bool write_nbt_data(uint32_t  & cursor, uint8_t * data, uint32_t size);
};

class TAG_Byte_Array: public TAG
{
    TAG_Byte length;
    uint8_t * byte_array;
public:
    uint8_t get_type(){return 7;}
    uint32_t get_size(){return length.payload_byte + length.get_size();}
    bool write_nbt_data(uint32_t  & cursor, uint8_t * data, uint32_t size);
};

class TAG_String: public TAG
{
    TAG_Short length;
    uint8_t * byte_array;
public:
    uint8_t get_type(){return 8;}
    uint32_t get_size(){return length.payload_short + length.get_size();}
    bool write_nbt_data(uint32_t  & cursor, uint8_t * data, uint32_t size);
};

class TAG_List: public TAG
{
    TAG_Byte tagId;
    TAG_Int length;
    TAG * tag_array;
public:
    uint8_t get_type(){return 9;}
    uint32_t get_size(){return ((length.payload_int * tag_array[1].get_size())  + length.get_size());}
    bool write_nbt_data(uint32_t  & cursor, uint8_t * data, uint32_t size);
};

class TAG_Compound: public TAG
{
    TAG_Int length;
    TAG * tag_array;
public:
    uint8_t get_type(){return 10;}
    uint32_t get_size();
    bool write_nbt_data(uint32_t  & cursor, uint8_t * data, uint32_t size);
};

class nbt_t
{
    TAG * root;
public:
    nbt_t(void);
    ~nbt_t(void);
};

