#include "ShelfUI.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

const std::string ShelfUI::QUEUE_THREAD = "queue-thread";
const std::string ShelfUI::DEQUEUE_THREAD = "dequeue-thread";

ShelfUI::ShelfUI():
    Observer("shelf") {}

ShelfUI::~ShelfUI()
{
}

std::string ShelfUI::toString(std::vector<MaintainerUI*>& maintainers)
{
    std::ostringstream result;
    result << "[SHELF]";
    
    for (std::string thread_id : this->queue_to_shelf)
    {
        auto maintainer = std::find_if(
            maintainers.begin(),
            maintainers.end(),
            [thread_id](auto maintainer_from_collection)
            {
                return maintainer_from_collection->getId() == thread_id;
            }
        );
        if (maintainer == maintainers.end())
            continue;
        result << " " << (*maintainer)->toString();
        maintainers.erase(maintainer);
    }
    
    return result.str();
}

void ShelfUI::update(const std::string message)
{
    std::istringstream parser(message);
    std::string mnemonic;
    parser >> mnemonic;

    if (mnemonic == ShelfUI::QUEUE_THREAD)
    {
        std::string thread_id;
        parser >> thread_id;

        this->queue_to_shelf.push_back(thread_id);
    }
    else if (mnemonic == ShelfUI::DEQUEUE_THREAD)
    {
        std::string thread_id;
        parser >> thread_id;

        auto it = std::remove(
            this->queue_to_shelf.begin(),
            this->queue_to_shelf.end(),
            thread_id
        );
        if (it != this->queue_to_shelf.end())
            this->queue_to_shelf.erase(it);
    }
}
