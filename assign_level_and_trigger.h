#ifndef ASSIGN_LEVEL_AND_TRIGGER_H
#define ASSIGN_LEVEL_AND_TRIGGER_H
int assign_level(ProcessedData data, const char *output_file);
void trigger_alert(int danger_level, const char *script, const char *date, const char *time);
#endif //ASSIGN_LEVEL_AND_TRIGGER_H
