#include "nbt_t.h"



int is_big_endian(void)
{
    union {
        uint32_t i;
        char c[4];
    } big_int = {0x01020304};

    return big_int.c[0] == 1; 
}


bool TAG_Byte::write_nbt_data(uint32_t  & cursor, uint8_t * data, uint32_t size)
{
    if (cursor > size-get_size())
        return 0;
    data[cursor] = payload_byte;
    cursor += 1;
    return 1;
}

bool TAG_Short::write_nbt_data(uint32_t  & cursor, uint8_t * data, uint32_t size)
{
    if (cursor > size-get_size())
        return 0;
    if(is_big_endian)
    for(int i = 0; i < get_size(); i++)
    {
        data[cursor] = byte_array [i];
        cursor++;
    }
    else
    for(int i = get_size()-1; i >= 0; i--)
    {
        data[cursor] = byte_array [i];
        cursor++;
    }
    return 1;
}

bool TAG_Int::write_nbt_data(uint32_t  & cursor, uint8_t * data, uint32_t size)
{
    if (cursor > size-get_size())
        return 0;
    if(is_big_endian)
    for(int i = 0; i < get_size(); i++)
    {
        data[cursor] = byte_array [i];
        cursor++;
    }
    else
    for(int i = get_size()-1; i >= 0; i--)
    {
        data[cursor] = byte_array [i];
        cursor++;
    }
    return 1;
}

bool TAG_Long::write_nbt_data(uint32_t  & cursor, uint8_t * data, uint32_t size)
{
    if (cursor > size-get_size())
        return 0;
    if(is_big_endian)
    for(int i = 0; i < get_size(); i++)
    {
        data[cursor] = byte_array [i];
        cursor++;
    }
    else
    for(int i = get_size()-1; i >= 0; i--)
    {
        data[cursor] = byte_array [i];
        cursor++;
    }
    return 1;
}

bool TAG_Float::write_nbt_data(uint32_t  & cursor, uint8_t * data, uint32_t size)
{
    if (cursor > size-get_size())
        return 0;
    if(is_big_endian)
    for(int i = 0; i < get_size(); i++)
    {
        data[cursor] = byte_array [i];
        cursor++;
    }
    else
    for(int i = get_size()-1; i >= 0; i--)
    {
        data[cursor] = byte_array [i];
        cursor++;
    }
    return 1;
}

bool TAG_Double::write_nbt_data(uint32_t  & cursor, uint8_t * data, uint32_t size)
{
    if (cursor > size-get_size())
        return 0;
    if(is_big_endian)
    for(int i = 0; i < get_size(); i++)
    {
        data[cursor] = byte_array [i];
        cursor++;
    }
    else
    for(int i = get_size()-1; i >= 0; i--)
    {
        data[cursor] = byte_array [i];
        cursor++;
    }
    return 1;
}

bool TAG_Byte_Array::write_nbt_data(uint32_t  & cursor, uint8_t * data, uint32_t size)
{
    if (cursor > size-get_size())
        return 0;
    length.write_nbt_data(cursor, data, size);
    for(uint32_t i = 0; i < length.payload_byte; i++)
    {
        data[cursor] = byte_array[i];
        cursor++;
    }
    return 1;
}

bool TAG_String::write_nbt_data(uint32_t  & cursor, uint8_t * data, uint32_t size)
{
    if (cursor > size-get_size())
        return 0;
    length.write_nbt_data(cursor, data, size);
    for(uint32_t i = 0; i < length.payload_short; i++)
    {
        data[cursor] = byte_array[i];
        cursor++;
    }
    return 1;
}

bool TAG_List::write_nbt_data(uint32_t  & cursor, uint8_t * data, uint32_t size)
{
    if (cursor > size-get_size())
        return 0;
    tagId.write_nbt_data(cursor, data, size);
    length.write_nbt_data(cursor, data, size);
    for(uint32_t i = 0; i < length.payload_int; i++)
    {
        tag_array[i].write_nbt_data(cursor, data, size);
    }
    return 1;
}

bool TAG_Compound::write_nbt_data(uint32_t  & cursor, uint8_t * data, uint32_t size)
{
    if (cursor > size-get_size())
        return 0;
    for(uint32_t i = 0; i < length.payload_int; i++)
    {
        data[cursor] = tag_array[i].get_type();
        cursor++;
        if(!(tag_array[i].label->write_nbt_data(cursor, data, size))) return 0;
        if(!(tag_array[i].write_nbt_data(cursor, data, size))) return 0;
    }
    return 1;
}

uint32_t TAG_Compound::get_size()
{
    uint32_t Templength = 0;
    for(int i = 0; i < length.payload_int; i++)
    {
        Templength++; //for the tag identifier.
        Templength += tag_array[i].label->get_size(); //for the tag name.
        Templength += tag_array[i].get_size(); //for the actual length of the tag.
    }
    Templength ++; //for the final closing byte.
    return Templength;
}


nbt_t::nbt_t(void)
{
}


nbt_t::~nbt_t(void)
{
}
