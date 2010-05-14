#include "ScriptedPch.h"
#include "eye_of_eternity.h"
#include "WorldPacket.h"
 
#define DISABLED_ENTER_MESSAGE "You cannot enter Eye of Eternity now"
#define EXIT_MAP 571
#define EXIT_X 3864
#define EXIT_Z 6987
#define EXIT_Y 152
 
 
struct instance_eye_of_eternity : public ScriptedInstance
{
    instance_eye_of_eternity(Map* pMap) : ScriptedInstance(pMap) {Initialize();}
 
    std::string strInstData;
    uint32 m_auiEncounter[MAX_ENCOUNTER];
    uint32 m_uiOutroCheck;
    uint32 m_uiMalygosPlatformData;
 
    GameObject* m_uiMalygosPlatform;
    GameObject* m_uiFocusingIris;
    GameObject* m_uiExitPortal;
 
    uint64 m_uiMalygosGUID;
    
 
    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
                
        m_uiMalygosGUID = 0;
        m_uiOutroCheck = 0; 
        m_uiMalygosPlatformData = 0;
        m_uiMalygosPlatform = NULL;
        m_uiFocusingIris = NULL;
        m_uiExitPortal = NULL;
    }
    
    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case NPC_MALYGOS:
                m_uiMalygosGUID = pCreature->GetGUID();
                break;
            default:
                break;
        }
    }
    
    void OnGameObjectCreate(GameObject *pGo, bool bAdd)
    {
        switch(pGo->GetEntry())
        {
            case 193070: m_uiMalygosPlatform = pGo; break;
            case 193958: //normal, hero 
            case 193960: m_uiFocusingIris = pGo; break;
            case 193908: m_uiExitPortal = pGo; break;
            default:
                break;
        }
    }
 
    bool IsEncounterInProgress() const
    {
        for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            if (m_auiEncounter[i] == IN_PROGRESS)
                return true;
 
        return false;
    }
 
    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
            case TYPE_MALYGOS:
                if(uiData == IN_PROGRESS)
                {
                  if(m_uiExitPortal)
                      m_uiExitPortal->Delete();
                  if(m_uiFocusingIris)
                      m_uiFocusingIris->Delete();                
                }
                m_auiEncounter[0] = uiData;
                break;
            case TYPE_OUTRO_CHECK:
                m_uiOutroCheck = uiData;
                break;
            case TYPE_DESTROY_PLATFORM:
                if(uiData == IN_PROGRESS && m_uiMalygosPlatform)
                    m_uiMalygosPlatform->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_DESTROYED);
                m_uiMalygosPlatformData = uiData;
            break;
        }
    }
 
    const char* Save()
    {
        OUT_SAVE_INST_DATA;
        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_uiOutroCheck;
 
        strInstData = saveStream.str();
        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
        return strInstData.c_str();
    }
 
    void Load(const char* chrIn)
    {
        if (!chrIn)
        {
            OUT_LOAD_INST_DATA_FAIL;
            return;
        }
 
        OUT_LOAD_INST_DATA(chrIn);
 
        std::istringstream loadStream(chrIn);
        loadStream >> m_auiEncounter[0] >> m_uiOutroCheck;
 
        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        {
            if (m_auiEncounter[i] == IN_PROGRESS)
                m_auiEncounter[i] = NOT_STARTED;
        }
 
        OUT_LOAD_INST_DATA_COMPLETE;
    }
 
    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
            case TYPE_MALYGOS:
                return m_auiEncounter[0];
            case TYPE_OUTRO_CHECK:
                return m_uiOutroCheck;
            case TYPE_DESTROY_PLATFORM:
                return m_uiMalygosPlatformData;
        }
        return 0;
    }
 
    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case NPC_MALYGOS:
                return m_uiMalygosGUID;
            default:
                return 0;
        }
        return 0;
    }
};
 
InstanceData* GetInstanceData_instance_eye_of_eternity(Map* pMap)
{
    return new instance_eye_of_eternity(pMap);
}
 
void AddSC_instance_eye_of_eternity()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_eye_of_eternity";
    newscript->GetInstanceData = &GetInstanceData_instance_eye_of_eternity;
    newscript->RegisterSelf();
}
