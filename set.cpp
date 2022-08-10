
#include "set.h"

size_t case_insensitive::hash(const std::string &s)
{
   size_t hashVal = 0;

   for (size_t i = 0; i < s.size(); i++)
   {
      size_t pw = 1;

      for (size_t k = 0; k < s.size() - i - 1; k++)
      {
         pw *= 31;
      }
      hashVal = hashVal + tolower(s[i]) * pw;
   }

   return hashVal;
}

bool case_insensitive::equal(const std::string &s1, const std::string &s2)
{
   if ((s1.size() != s2.size()))
      return false;

   for (size_t i = 0; i < s1.size(); i++)
   {
      if (tolower(s1[i]) != tolower(s2[i]))
         return false;
   }

   return true;
}

bool set::insert_norehash(const std::string &s)
{
   size_t hashVal = case_insensitive::hash(s);
   size_t bucketIndex = hashVal % buckets.size();

   for (auto it = buckets[bucketIndex].begin(); it != buckets[bucketIndex].end(); ++it)
   {
      if (case_insensitive::equal(*it, s) == true)
         return false;
   }

   buckets[bucketIndex].push_back(s);
   set_size++;
   return true;
}

void set::consider_rehash()
{

   if (loadfactor() >= max_load_factor)
   {
      size_t newSize = buckets.size() * 2;

      std::vector<buckettype> newBuckets(newSize);

      for (size_t i = 0; i < buckets.size(); i++)
      {
         for (auto it = buckets[i].begin(); it != buckets[i].end(); ++it)
         {
            size_t bucketIndex;
            size_t hashVal = case_insensitive::hash(*it);
            bucketIndex = hashVal % newSize;
            newBuckets[bucketIndex].push_back(*it);
         }
      }

      buckets = newBuckets;
   }
}

set::set(std::initializer_list<std::string> init, double max_load_factor)
    : set_size(0),
      max_load_factor(max_load_factor),
      buckets(std::vector<buckettype>(8))
{

   for (const auto &str : init)
   {
      insert(str);
   }
}

bool set::contains(const std::string &s) const
{
   size_t hashVal = case_insensitive::hash(s);
   size_t bucketIndex = hashVal % buckets.size();

   for (auto it = buckets[bucketIndex].begin(); it != buckets[bucketIndex].end(); ++it)
   {
      if (case_insensitive::equal(*it, s))
         return true;
   }

   return false;
}

bool set::insert(const std::string &s)
{
   if (insert_norehash(s))
   {
      consider_rehash();
      return true;
   }
   else
   {
      return false;
   }
}

bool set::remove(const std::string &s)
{
   size_t hashVal = case_insensitive::hash(s);
   size_t bucketIndex = hashVal % buckets.size();

   for (auto it = buckets[bucketIndex].begin(); it != buckets[bucketIndex].end(); ++it)
   {
      if (*it == s)
      {
         buckets[bucketIndex].erase(it);
         set_size--;
         return true;
      }
   }

   return false;
}

size_t set::size() const
{
   return set_size;
}

bool set::empty() const
{
   if (set_size == 0)
      return true;

   return false;
}

void set::clear()
{
   for (size_t i = 0; i < buckets.size(); i++)
   {
      buckets[i].clear();
   }

   set_size = 0;
}

void set::print(std::ostream &out) const
{
   for (size_t i = 0; i < buckets.size(); i++)
   {
      out << "bucket[" << i << "]"
          << ": {";
      bool firstInList = true;
      for (auto it = buckets[i].begin(); it != buckets[i].end(); ++it)
      {
         if (firstInList == true)
         {
            out << *it;
            firstInList = false;
         }
         else
         {
            out << ", " << *it;
         }
      }
      out << "} \n";
   }
}
