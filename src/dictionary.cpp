#include "dictionary.h"
#include "tag.h"

Dictionary::Dictionary(QString name, QObject *parent)
  : QObject(parent)
{
  dictionary = new ctemplate::TemplateDictionary(name.toStdString());
  deleteDictionary = true;
}

Dictionary::Dictionary(ctemplate::TemplateDictionary *dictionary, QObject *parent)
  : QObject(parent), dictionary(dictionary)
{
  deleteDictionary = false;
}

Dictionary::~Dictionary()
{
  if (deleteDictionary) {
    delete dictionary;
  }
}

Dictionary *Dictionary::addSectionDictionary(QString name)
{
  ctemplate::TemplateDictionary *sectionDictionary =
    dictionary->AddSectionDictionary(name.toStdString());

  return new Dictionary(sectionDictionary, this);
}

Dictionary *Dictionary::addIncludeDictionary(QString name, QString templateName)
{
  ctemplate::TemplateDictionary *includeDictionary =
    dictionary->AddIncludeDictionary(name.toStdString());
  includeDictionary->SetFilename(templateName.toStdString());
  return new Dictionary(includeDictionary, this);
}

void Dictionary::setValue(QString name, QString value)
{
  dictionary->SetValue(name.toStdString(), value.toStdString());
}

void Dictionary::setValue(QString name, int value)
{
  dictionary->SetIntValue(name.toStdString(), value);
}

void Dictionary::showSection(QString name)
{
  dictionary->ShowSection(name.toStdString());
}

void Dictionary::addActivity(const Activity &activity)
{
  setValue("id", activity.id());
  setValue("name", activity.name());
  setValue("nameWithProject", activity.nameWithProject());
  setValue("startedAtISO8601", activity.startedAtISO8601());
  setValue("startedAtHM", activity.startedAtHM());
  setValue("startedAtMDY", activity.startedAtMDY());
  setValue("endedAtHM", activity.endedAtHM());
  setValue("endedAtMDY", activity.endedAtMDY());
  setValue("dayMDY", activity.dayMDY());

  int duration = activity.duration();
  int minutes = duration / 60;
  setValue("duration", duration);
  setValue("durationHours", minutes / 60);
  setValue("durationMinutes", minutes % 60);
  setValue("durationInWords", activity.durationInWords());

  QString projectName = activity.projectName();
  setValue("projectName", projectName.isEmpty() ? "unsorted" : projectName);

  setValue("tagNames", activity.tagNames());
  QList<Tag> tags = activity.tags();
  Dictionary *tagsDictionary = addIncludeDictionary("tags", "_tags.html");
  for (int i = 0; i < tags.size(); i++) {
    Dictionary *tagDictionary = tagsDictionary->addSectionDictionary("tag");
    tagDictionary->setValue("id", tags[i].id());
    tagDictionary->setValue("name", tags[i].name());
  }

  if (activity.isRunning()) {
    showSection("running");
  }
  else {
    showSection("notRunning");
  }

  if (activity.isUntimed()) {
    showSection("untimed");
  }
  else {
    showSection("timed");
  }
}

void Dictionary::addActivitySection(const Activity &activity)
{
  addActivitySection(activity, "activity");
}

void Dictionary::addActivitySection(const Activity &activity, const QString &sectionName)
{
  Dictionary *section = addSectionDictionary("activity");
  section->addActivity(activity);
}

void Dictionary::addActivitySection(const QList<Activity> &activities)
{
  addActivitySection(activities, "activity");
}

void Dictionary::addActivitySection(const QList<Activity> &activities, const QString &sectionName)
{
  for (int i = 0; i < activities.size(); i++) {
    addActivitySection(activities[i], sectionName);
  }
}

ctemplate::TemplateDictionary *Dictionary::object()
{
  return dictionary;
}
