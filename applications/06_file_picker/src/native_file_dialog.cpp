#include "native_file_dialog.h"
#include <iostream>

#ifdef __linux__
#include <gtk/gtk.h>
#elif _WIN32
#include <windows.h>
#include <commdlg.h>
#elif __APPLE__
// macOS implementation would go here
#endif

std::string NativeFileDialog::openFile(const std::string& title,
                                      const std::vector<FileFilter>& filters) {
#ifdef __linux__
    return showGtkFileDialog(false, false, title, "", filters);
#elif _WIN32
    // Windows implementation
    char filename[MAX_PATH] = "";
    
    OPENFILENAMEA ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = filename;
    ofn.nMaxFile = sizeof(filename);
    ofn.lpstrTitle = title.c_str();
    ofn.Flags = OFN_FILEMUSTEXIST;
    
    if (GetOpenFileNameA(&ofn)) {
        return std::string(filename);
    }
    return "";
#else
    return "";
#endif
}

std::vector<std::string> NativeFileDialog::openFiles(const std::string& title,
                                                    const std::vector<FileFilter>& filters) {
#ifdef __linux__
    return showGtkMultiFileDialog(title, filters);
#else
    // Fallback to single file selection
    std::vector<std::string> result;
    std::string file = openFile(title, filters);
    if (!file.empty()) {
        result.push_back(file);
    }
    return result;
#endif
}

std::string NativeFileDialog::openDirectory(const std::string& title) {
#ifdef __linux__
    return showGtkFileDialog(false, true, title, "", {});
#else
    return "";
#endif
}

std::string NativeFileDialog::saveFile(const std::string& title,
                                      const std::string& defaultName,
                                      const std::vector<FileFilter>& filters) {
#ifdef __linux__
    return showGtkFileDialog(true, false, title, defaultName, filters);
#elif _WIN32
    char filename[MAX_PATH] = "";
    strcpy_s(filename, defaultName.c_str());
    
    OPENFILENAMEA ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = filename;
    ofn.nMaxFile = sizeof(filename);
    ofn.lpstrTitle = title.c_str();
    ofn.Flags = OFN_OVERWRITEPROMPT;
    
    if (GetSaveFileNameA(&ofn)) {
        return std::string(filename);
    }
    return "";
#else
    return "";
#endif
}

#ifdef __linux__
std::string NativeFileDialog::showGtkFileDialog(bool save,
                                               bool directory,
                                               const std::string& title,
                                               const std::string& defaultPath,
                                               const std::vector<FileFilter>& filters) {
    gtk_init(nullptr, nullptr);
    
    GtkFileChooserAction action;
    const gchar* accept_button;
    
    if (directory) {
        action = GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER;
        accept_button = "_Select";
    } else if (save) {
        action = GTK_FILE_CHOOSER_ACTION_SAVE;
        accept_button = "_Save";
    } else {
        action = GTK_FILE_CHOOSER_ACTION_OPEN;
        accept_button = "_Open";
    }
    
    GtkWidget* dialog = gtk_file_chooser_dialog_new(
        title.c_str(),
        nullptr,
        action,
        "_Cancel", GTK_RESPONSE_CANCEL,
        accept_button, GTK_RESPONSE_ACCEPT,
        nullptr
    );
    
    if (save && !defaultPath.empty()) {
        gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), defaultPath.c_str());
    }
    
    // Add file filters
    for (const auto& filter : filters) {
        GtkFileFilter* gtk_filter = gtk_file_filter_new();
        gtk_file_filter_set_name(gtk_filter, filter.description.c_str());
        
        for (const auto& ext : filter.extensions) {
            if (ext == "*") {
                gtk_file_filter_add_pattern(gtk_filter, "*");
            } else {
                std::string pattern = "*." + ext;
                gtk_file_filter_add_pattern(gtk_filter, pattern.c_str());
            }
        }
        
        gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), gtk_filter);
    }
    
    std::string result;
    gint res = gtk_dialog_run(GTK_DIALOG(dialog));
    
    if (res == GTK_RESPONSE_ACCEPT) {
        char* filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        if (filename) {
            result = filename;
            g_free(filename);
        }
    }
    
    gtk_widget_destroy(dialog);
    
    // Process GTK events
    while (gtk_events_pending()) {
        gtk_main_iteration();
    }
    
    return result;
}

std::vector<std::string> NativeFileDialog::showGtkMultiFileDialog(const std::string& title,
                                                                 const std::vector<FileFilter>& filters) {
    std::vector<std::string> results;
    
    gtk_init(nullptr, nullptr);
    
    GtkWidget* dialog = gtk_file_chooser_dialog_new(
        title.c_str(),
        nullptr,
        GTK_FILE_CHOOSER_ACTION_OPEN,
        "_Cancel", GTK_RESPONSE_CANCEL,
        "_Open", GTK_RESPONSE_ACCEPT,
        nullptr
    );
    
    // Enable multiple selection
    gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(dialog), TRUE);
    
    // Add file filters
    for (const auto& filter : filters) {
        GtkFileFilter* gtk_filter = gtk_file_filter_new();
        gtk_file_filter_set_name(gtk_filter, filter.description.c_str());
        
        for (const auto& ext : filter.extensions) {
            if (ext == "*") {
                gtk_file_filter_add_pattern(gtk_filter, "*");
            } else {
                std::string pattern = "*." + ext;
                gtk_file_filter_add_pattern(gtk_filter, pattern.c_str());
            }
        }
        
        gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), gtk_filter);
    }
    
    gint res = gtk_dialog_run(GTK_DIALOG(dialog));
    
    if (res == GTK_RESPONSE_ACCEPT) {
        GSList* filenames = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(dialog));
        GSList* iter = filenames;
        
        while (iter != nullptr) {
            char* filename = static_cast<char*>(iter->data);
            if (filename) {
                results.push_back(std::string(filename));
                g_free(filename);
            }
            iter = iter->next;
        }
        
        g_slist_free(filenames);
    }
    
    gtk_widget_destroy(dialog);
    
    // Process GTK events
    while (gtk_events_pending()) {
        gtk_main_iteration();
    }
    
    return results;
}
#endif