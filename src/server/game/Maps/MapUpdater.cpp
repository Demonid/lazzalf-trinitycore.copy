#include "MapUpdater.h"
#include "DelayExecutor.h"
#include "Map.h"
#include "DatabaseEnv.h"
#include "MapManager.h"
#include "World.h"

#include <ace/Guard_T.h>
#include <ace/Method_Request.h>
#include <ace/Stack_Trace.h>

class WDBThreadStartReq1 : public ACE_Method_Request
{
    public:

        WDBThreadStartReq1()
        {
        }

        virtual int call()
        {
            return 0;
        }
};

class WDBThreadEndReq1 : public ACE_Method_Request
{
    public:

        WDBThreadEndReq1()
        {
        }

        virtual int call()
        {
            return 0;
        }
};

UNORDERED_MAP< uint32, std::pair<uint32, uint32> > m_crashcounter;

class MapUpdateRequest : public ACE_Method_Request
{
    private:

        Map& m_map;
        MapUpdater& m_updater;
        ACE_UINT32 m_diff;

    public:

        MapUpdateRequest(Map& m, MapUpdater& u, ACE_UINT32 d)
            : m_map(m), m_updater(u), m_diff(d)
        {
        }

        virtual int call()
        {
            if (sWorld->getBoolConfig(CONFIG_CRASH_RECOVER_ENABLE))
            {
                signal(SIGSEGV, HandleCrash);
                m_map.m_updater = ACE_Based::Thread::currentId();
                m_map.Update (m_diff);
                if (m_crashcounter[m_map.GetId()].second)
                {
                    if(m_diff >= m_crashcounter[m_map.GetId()].second)
                    {
                        m_crashcounter[m_map.GetId()].second = 0;
                        m_crashcounter[m_map.GetId()].first = 0;
                    }
                    else
                        m_crashcounter[m_map.GetId()].second -= m_diff;
                }
                m_updater.update_finished ();
                return 0;
            }
            else
            {
                m_map.Update (m_diff);
                m_updater.update_finished ();
                return 0;
            }
        }

  static void HandleCrash(int s)
  {
    if (Map *map = sMapMgr->FindMapByThread(ACE_Based::Thread::currentId()))
    {
        if (++m_crashcounter[map->GetId()].first < sWorld->getIntConfig(CONFIG_UINT32_MAX_CRASH_COUNT))
        {
            ACE_Stack_Trace st(-5);

            if (map->Instanceable())
                sLog->outError("Trovato crash nell'istanza %i della mappa %i", ((InstanceMap*)map)->GetInstanceId(), map->GetId());
            else
                sLog->outError("Trovato crash nella mappa %i", map->GetId());
            sLog->outError("Crash numero: %i", m_crashcounter[map->GetId()].first);
            m_crashcounter[map->GetId()].second = sWorld->getIntConfig(CONFIG_UINT32_CRASH_COUNT_RESET);

            sLog->outError("Stack Trace:\n%s", st.c_str());
            sWorld->ShutdownServ(180, SHUTDOWN_MASK_RESTART | SHUTDOWN_MASK_IDLE, RESTART_EXIT_CODE);
            map->Wipe();
            sMapMgr->m_updater.update_finished_wrapper();

            if (sMapMgr->m_updater.respawn() == -1)
            {
                sLog->outError("Impossibile ripristinare il thread. shutdown in corso...");
                abort();
            }

            ACE_Thread::exit(0);
        }
        else
            sLog->outError("Numero massimo di crash raggiunto. shutdown in corso...");
    }
    else
        sLog->outError("Trovato crash esterno all'update delle mappe.");
  }
};

MapUpdater::MapUpdater():
m_executor(), m_mutex(), m_condition(m_mutex), pending_requests(0)
{
}

MapUpdater::~MapUpdater()
{
    deactivate();
}

int MapUpdater::activate(size_t num_threads)
{
    return m_executor.activate((int)num_threads, new WDBThreadStartReq1, new WDBThreadEndReq1);
}

int MapUpdater::deactivate()
{
    wait();

    return m_executor.deactivate();
}

int MapUpdater::wait()
{
    ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_mutex, -1);

    while (pending_requests > 0)
        m_condition.wait();

    return 0;
}

int MapUpdater::schedule_update(Map& map, ACE_UINT32 diff)
{
    ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_mutex, -1);

    ++pending_requests;

    if (m_executor.execute(new MapUpdateRequest(map, *this, diff)) == -1)
    {
        ACE_DEBUG((LM_ERROR, ACE_TEXT("(%t) \n"), ACE_TEXT("Failed to schedule Map Update")));

        --pending_requests;
        return -1;
    }

    return 0;
}

bool MapUpdater::activated()
{
    return m_executor.activated();
}

void MapUpdater::update_finished()
{
    ACE_GUARD(ACE_Thread_Mutex, guard, m_mutex);

    if (pending_requests == 0)
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("(%t)\n"), ACE_TEXT("MapUpdater::update_finished BUG, report to devs")));
        return;
    }

    --pending_requests;

    m_condition.broadcast();
}
