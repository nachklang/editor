#include "LevelView.h"

namespace editor {

void LevelView::onActorActivated(const std::shared_ptr<Actor> &index)
{
    if(index)
    {
        if (m_activatedActor.empty())
        {
            m_activatedActor.push_back(index);
        }
        else
        {
            if (m_activatedActor[0] != index)
            {
                m_activatedActor[0]->toggleActivated();
                m_activatedActor[0] = index;
            }
            else
            {
                m_activatedActor.pop_back();
            }

            if(!m_activatedActor.size())
            {
                emit m_tracker->activated(nullptr);
            }
        }
    }

}

}
