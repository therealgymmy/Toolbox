How to Use
==========

A Simple Example
----------------

    void foo() {
        vector<int*> c1;
        map<int, int*> c2;
        map<int, pair<string*, string*>> c3;
 
        scope(exit, {
            release_container(c1);
            release_container(c2);
            release_container_with_deleter(c3, itr, {
                delete itr->second.first;
                delete itr->second.second;
            });
        });
 
        // add stuff to c1, c2, c3
    }
    // resources released properly at this point

Another Example
---------------

    void rollback(vector<data*>&) noexcept;
    void doStuffAndMayThrow(vector<data*>&, map<int, set<account*>>&);
    void moveToClient(map<int, set<account*>>&);

    void foo2() {
        vector<data*> dataCache;
        map<int, set<account*>> uniqueAccountMap;

        // on failure, clean up the uniqueAccountMap and rollback dataCache
        scope(failure, {
            release_container_with_deleter(uniqueAccountMap, itr, {
                set<account*> &temp = itr->second;
                for (auto itr2 : temp) delete *itr;
            });
            rollback(dataCache);
        });

        // clean up dataCache, regardless of failure or success
        scope(exit, {
            release_container(dataCache);
        });

        // now, do some stuff!

        doStuffAndMayThrow(dataCache, uniqueAccountMap);

        moveToClient(uniqueAccountMap);
    }

Macro: `scope`
--------------
    
###Syntax:
    
    scope(condition, {
        // code block
    });

    condition: enum of type ScopeGuardType
               exit    -> code block execute on exit from scope
               success -> code block execute on normal exit from scope
               failure -> code block execute on exception-induced exit from scope
  
###Example:
        
    scope(exit, {
        delete dynamicAllocatedInteger;
        delete dynamicAllocatedObject;
    });
  
    scope(success, {
        if (debug) saveToLog();
    });
  
    scope(failure, {
        rollback();
        delete cache;
    });
  
  
Macro: `release_container`
--------------------------
    
###Syntax:
    
    release_container(container_name);

    
Macro: `release_container_with_deleter`
---------------------------------------
    
###Syntax:
    
    release_container_with_deleter(container_name, iterator_name {
        // custom clean up code
    });
  
    container_name: name of your container variable
    iterator_name: name of the iterator that you will address in the code block
    
###Example:
  
    map<int*, pair<int*, object*>> map_container;
    release_container_with_deleter(map_container, itr, {
        delete itr->first;
        delete itr->second.first;
        delete itr->second.second;
    });
