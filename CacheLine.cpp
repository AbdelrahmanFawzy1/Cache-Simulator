#include <CacheLine.h>

#define LM1  0x8000000000000000

CacheStats::CacheStats(){
    hits = 0;
    misses = 0;
    cache_reads = 0;
    cache_writes = 0;
    mem_reads = 0;
    mem_writes = 0;
}
void CacheStats::incHits(){
    hits++;
}
void CacheStats::incMisses(){
    misses++;
}
void CacheStats::incCacheReads(){
    cache_reads++;
}
void CacheStats::incCacheWrites(){
    cache_writes++;
}
void CacheStats::incMemReads(){
    mem_reads++;
}
void CacheStats::incMemWrites(){
    mem_writes++;
}
void CacheStats::printStats (unsigned int crc,unsigned int cwc ,unsigned int mrc ,unsigned int mwc)
{
    printf ("Total Misses: %llu\n",misses);
    printf ("Total Hits: %llu\n",hits);
    printf ("Memory Read Access Attempts: %llu\n",cache_reads);
    printf ("Memory Write Access Attempts: %llu\n",cache_writes);
    printf ("Total Memory Access Attempts: %llu\n",cache_reads+cache_writes);
    printf ("Memory Read Access: %llu\n",mem_reads);
    printf ("Memory Write Access: %llu\n",mem_writes);
    printf ("Total # of cycles for cache read: %llu\n",cache_reads*crc);
    printf ("Total # of cycles for cache writes: %llu\n",cache_writes*cwc);
    printf ("Total # of cycles for cache access: %llu\n",(cache_reads*crc)+(cache_writes*cwc));
    printf ("Total # of cycles for memory read: %llu\n",mem_reads*mrc);
    printf ("Total # of cycles for memory writes: %llu\n",mem_writes*mwc);
    printf ("Total # of cycles for memory access: %llu\n",(mem_reads*mrc)+(mem_writes*mwc));
    printf ("Total # of cycles for memory/cache access: %llu\n",(cache_reads*crc)+(cache_writes*cwc)+(mem_reads*mrc)+(mem_writes*mwc));
}

CacheStats::~CacheStats(){

}


CacheLine::CacheLine (CacheStats * p_cacheStats,bool _wb)
{
    v = false;
    counter = 0;
    tag =0;
    wb = _wb;
    cacheStats = p_cacheStats;
}
bool CacheLine::valid()
{
    return v;
}
uint64_t CacheLine::getTag()
{
    return tag;
}

uint64_t CacheLine::getCounter()
{
    return counter;
}
void CacheLine::handle_miss (uint64_t p_tag)
{
	tag=p_tag;   
    v = true;
    counter = 0;
    if(modified_bit&& wb)
    cacheStats->incMemWrites();
    modified_bit=false;
  
}
void CacheLine::access (uint64_t p_tag,char mode)
{
    if (mode==READ_ACCESS){
    	if (p_tag==getTag()&&(this->valid())){cacheStats->incCacheReads();cacheStats->incHits();}
    	else{
        cacheStats->incMisses();
        handle_miss(p_tag);
		cacheStats->incMemReads();
        cacheStats->incCacheReads();
		}
		
	}else{    //write
		if (p_tag==getTag()&&(this->valid())){
        cacheStats->incCacheWrites();
        cacheStats->incHits();
        if (!wb)cacheStats->incMemWrites();
         modified_bit=true;
		
        }else{  
            handle_miss(p_tag);
			cacheStats->incMemReads();
	        cacheStats->incCacheWrites();
            if (!wb)cacheStats->incMemWrites();
             modified_bit=true;
		}}
}
unsigned short  CacheLine::check (uint64_t p_tag)
{
    if(valid()==false){return CL_NOT_VALID;}
    else if(p_tag==getTag()){
    	return TAG_MATCH;	
	}else{
		return TAG_MISMATCH;	
	}
}
void CacheLine::updateCounter (bool access)
{
   counter=counter>>1;
   if (access){
   	int x=1;
   	x=x<<7;
   counter|=x;
   }
}
CacheLine::~CacheLine()
{

}
