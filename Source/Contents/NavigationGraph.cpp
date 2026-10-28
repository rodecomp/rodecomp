#include "Contents/NavigationManager.h"

// OFFSET: 0x00B89990
// FUNCTION: fn_19390
int CNaviData::FindRoute(std::vector<CNavi_Link*>* route, bool direct, bool allowGates, bool includeObjects, bool allowWarps, int side) {
  for (NaviDataEntry& entry : this->entries) {
    entry.previous = -1;
    entry.link = nullptr;
    entry.cost = 0;
  }
  this->entries.clear();

  for (CNaviInfo* destination : this->destinations) {
    destination->SetVisited(false);
  }

  if (includeObjects) {
    for (CNavi_Link* object : this->objects) {
      object->SetDepth(0);
      NaviDataEntry entry = {-1, object, 1};
      this->entries.push_back(entry);
    }
  }

  for (NaviLink& start : this->startLinks) {
    CNavi_Link* link = start.link;
    const int type = link->GetType();
    bool accept;
    if (type == 204) {
      if (!allowGates) {
        continue;
      }
      accept = !link->GetLinkedInfo()->IsVisited();
    } else if (type == 205) {
      if (!allowWarps) {
        continue;
      }
      accept = !link->GetLinkedInfo()->IsVisited();
    } else if (type == 202 || type == 203) {
      if (allowGates != true) {
        continue;
      }
      accept = type == 202 ? side == 2 : side == 1;
    } else {
      accept = !link->GetLinkedInfo()->IsVisited();
    }
    if (accept) {
      start.link->SetDepth(0);
      NaviDataEntry entry = {-1, start.link, start.cost};
      this->entries.push_back(entry);
    }
  }

  int first = 0;
  int depth = 1;
  int count = static_cast<int>(this->entries.size());
  int best = -1;
  int result;

  for (;;) {
    for (int i = first; i < count; ++i) {
      NaviDataEntry& entry = this->entries[i];
      if (this->info != entry.link->GetLinkedInfo()) {
        continue;
      }

      int cost;
      if (direct) {
        cost = entry.cost;
      } else {
        bool found = false;
        for (NaviLink& end : this->endLinks) {
          if (entry.link == end.link) {
            cost = end.cost + entry.cost;
            found = true;
            break;
          }
        }
        if (!found) {
          continue;
        }
      }

      if (this->range > cost) {
        this->range = cost;
        best = i;
      }

      result = this->range;
      std::vector<CNavi_Link*> path;
      for (int index = best; index != -1; index = this->entries[index].previous) {
        path.push_back(this->entries[index].link);
      }
      route->clear();
      for (int index = static_cast<int>(path.size()) - 1; index >= 0; --index) {
        route->push_back(path[index]);
      }
      goto done;
    }

    for (int i = first; i < count; ++i) {
      CNavi_Link* current = this->entries[i].link;
      for (std::vector<NaviLink>::iterator it = current->LinkedBegin(); it != current->LinkedEnd(); ++it) {
        CNavi_Link* next = it->link;
        if (next == nullptr) {
          continue;
        }

        if (depth == next->depth) {
          const int cost = it->cost + this->entries[i].cost;
          for (std::size_t j = count; j < this->entries.size(); ++j) {
            if (next == this->entries[j].link) {
              if (this->entries[j].cost > cost) {
                this->entries[j].previous = i;
                this->entries[j].link = next;
                this->entries[j].cost = cost;
              }
              break;
            }
          }
          continue;
        }

        if (depth < next->depth || next->depth != -1) {
          continue;
        }

        const int type = next->GetType();
        bool accept;
        if (type == 204) {
          if (!allowGates) {
            continue;
          }
          accept = !next->GetLinkedInfo()->IsVisited();
        } else if (type == 205) {
          if (!allowWarps) {
            continue;
          }
          accept = !next->GetLinkedInfo()->IsVisited();
        } else if (type == 202 || type == 203) {
          if (allowGates != true) {
            continue;
          }
          accept = type == 202 ? side == 2 : side == 1;
        } else {
          accept = !next->GetLinkedInfo()->IsVisited();
        }
        if (accept) {
          next->SetDepth(depth);
          NaviDataEntry entry = {i, next, it->cost + this->entries[i].cost};
          this->entries.push_back(entry);
        }
      }
    }

    first = count;
    count = static_cast<int>(this->entries.size());
    if (first == count) {
      break;
    }
    ++depth;
    if (depth >= 100) {
      break;
    }
  }
  result = 0;

done:
  this->range = 50000;
  for (NaviDataEntry& entry : this->entries) {
    entry.previous = -1;
    entry.link = nullptr;
    entry.cost = 0;
  }
  this->entries.clear();
  this->ResetLinkDepths();
  return result;
}

// OFFSET: 0x00B8A720
// FUNCTION: fn_19394
void CNaviData::ResetLinkDepths() {
  for (CNaviInfo* destination : this->destinations) {
    std::vector<CNavi_Link*>::iterator end = destination->LinksEnd();
    for (std::vector<CNavi_Link*>::iterator it = destination->LinksBegin(); it != end; ++it) {
      (*it)->SetDepth(-1);
    }
  }
}
