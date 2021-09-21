#include <vector>
#include <queue>
#include <dcore/managers/GuiManager.hpp>
#include <string>

template<typename T>
class List
{
public:
    void Add(const T &v);
    void Clear();
    void Remove(size_t index);
    size_t Size() const;
    T &operator[](size_t index);
    typename std::vector<T>::iterator begin();
    typename std::vector<T>::iterator end();
};

template<typename T>
class Queue
{
public:
    void Add(const T &v);
    void Clear();
    size_t Size() const;
    typename std::vector<T>::iterator begin();
    typename std::vector<T>::iterator end();
};

using String = std::string;

class Quest
{
public:
    const String &GetTitle() const;
    const String &GetDescription() const;
};

class Player
{
public:
    void AddQuest(Quest *q);
};

class Manager
{
    virtual void OnDrawGui(Player *player, GuiManager *gui) {}
};

class QuestManager : Manager
{
    struct QuestInfo { Player *player; List<Quest*> quests; };
    List<QuestInfo> pendingQuests;

    void AskQuests(Player *player, const List<Quest*> &quests)
    {
        pendingQuests.Add(QuestInfo { player, quests });
    }

    /* Вызовается когда надо рисовать интерфейс. */
    void OnDrawGui(Player *player, GuiManager *gui)
    {
        DrawPendingQuests(player, gui);
        // DrawCurrentQuests(player, gui);
    }

    /* Рисует окна с квестами. */
    void DrawPendingQuests(Player *player, GuiManager *gui)
    {
        int index = 0; List<int> toBeRemoved;
        for(auto &qinfo : pendingQuests)
        {
            if(qinfo.quests.Size() == 0) { DrawQuest(player, gui, qinfo); continue; }
            gui->BeginWindow();
            gui->BeginListChooser();
            Quest *selectedQuest = nullptr;
            for(auto quest : qinfo.quests)
            {
                if(gui->Selectable(quest->GetTitle().c_str(), false))
                selectedQuest = quest;
            }
            
            if(selectedQuest != nullptr) 
            {
                qinfo.quests.Clear();
                qinfo.quests.Add(selectedQuest);
            }
            gui->EndListChooser();
            gui->BeginBottomBar();
            if(gui->Button("Close")) { toBeRemoved.Add(index); }
            gui->EndBottomBar();
            gui->EndWindow();
            index += 1;
        }
        for(auto i : toBeRemoved) pendingQuests.Remove(i);
    }

    void DrawQuest(Player *player, GuiManager *gui, QuestInfo &qinfo)
    {
        gui->BeginWindow();
        Quest *q = qinfo.quests[0];
        gui->TextTitle(q->GetTitle().c_str());
        gui->LongText(q->GetDescription().c_str());
        DrawReqsPanel(player, gui, q);
        gui->BeginBottomBar();
        if(gui->Button("Accept")) { player->AddQuest(q); }
        if(gui->Button("Decline")) { qinfo.quests.Clear(); }
        gui->EndBottomBar();
        gui->EndWindow();  
    }

    void DrawReqsPanel(Player *player, GuiManager *gui, Quest *q);
};

