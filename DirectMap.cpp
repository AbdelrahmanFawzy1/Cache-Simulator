#include <DirectMap.h>
#include <Utils.h>

DirectMapProbe::DirectMapProbe (){
    directMap = NULL;
    counter = 0;
    status = NO_STATUS;
}
void DirectMapProbe::down(){
    lock.lock();
}
void DirectMapProbe::up ()
{
    lock.unlock();
}
void DirectMapProbe::setDirectMap(DirectMap * p_directMap)
{
    directMap = p_directMap;
}
void DirectMapProbe::setCounter (uint64_t p_counter)
{
    counter = p_counter;
}
void DirectMapProbe::setStatus (unsigned short p_status)
{
    status = p_status;
}

void DirectMapProbe::setCacheLineIndex(uint16_t p_cache_line_index)
{
    cache_line_index = p_cache_line_index;
}


DirectMap * DirectMapProbe::getDirectMap(){
    return directMap;
}
uint64_t DirectMapProbe::getCounter ()
{
    return counter;
}
unsigned short DirectMapProbe::getStatus()
{
    return status;
}

uint16_t DirectMapProbe::getCacheLineIndex ()
{
    return cache_line_index;
}

DirectMapProbe::~DirectMapProbe (){

}


DirectMap::DirectMap (CacheStats * p_cacheStats,uint16_t p_cache_line_size , uint16_t p_cache_size, bool _wb)
{
   cache_line_size=p_cache_line_size;
   cache_size=p_cache_size;
   cs_bits=log2(cache_size);
   ls_bits=log2(cache_line_size);

   for ( uint16_t i = 0 ; i < cache_size ; i++){
      CacheLine* new_line = new CacheLine (p_cacheStats,_wb);
      cache.insert(std::pair<uint16_t, CacheLine*>(i,new_line) );
}
}
void DirectMap::access (uint64_t full_address,char mode)
{
	uint16_t index = Utils::extract_bits(full_address,cs_bits,ls_bits);
    uint64_t tag = Utils::extract_bits(full_address,64-(cs_bits+ls_bits),cs_bits+ls_bits);
    cache[index]->access(tag,mode);
}
void DirectMap::check (uint64_t full_address,DirectMapProbe * p_directMapProbe)
{
  
   uint16_t index = Utils::extract_bits(full_address,cs_bits,ls_bits);
    uint64_t tag = Utils::extract_bits(full_address,64-(cs_bits+ls_bits),cs_bits+ls_bits);
    p_directMapProbe->down();
    unsigned short var =cache[index]->check(tag);
         if ( ( var == TAG_MATCH) ||
        (var == TAG_MISMATCH && cache[index]->getCounter() < p_directMapProbe->getCounter() && ( p_directMapProbe->getDirectMap() == NULL || p_directMapProbe->getStatus()==TAG_MISMATCH
          || p_directMapProbe->getStatus() == NO_STATUS))|| ( var == CL_NOT_VALID && (p_directMapProbe->getDirectMap() == NULL || p_directMapProbe->getStatus() != TAG_MATCH))
        )
   {
        p_directMapProbe->setCounter(cache[index]->getCounter());
        p_directMapProbe->setDirectMap(this);
        p_directMapProbe->setCacheLineIndex(index);
        p_directMapProbe->setStatus(var);
    } else p_directMapProbe->setDirectMap(NULL);
    
    p_directMapProbe->up();
}
void DirectMap::updateCounter (uint16_t index,bool access)
{
    cache[index]->updateCounter(access);
}
uint64_t DirectMap::getCounter (uint16_t index)
{
    return cache[index]->getCounter();

}

DirectMap::~DirectMap()
{
    for ( uint16_t i = 0 ; i < cache_size ; i++)
        delete (cache[i]);
}

