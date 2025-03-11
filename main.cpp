#include <gtk/gtk.h>
#include <iostream>
#include <cstdlib>
#include <vector>

class PDFPrinter {
public:
    static void print_pdf(GtkWidget *widget, gpointer data) {
        GtkWidget *parent = GTK_WIDGET(data);
        GtkComboBoxText *printer_combo = GTK_COMBO_BOX_TEXT(g_object_get_data(G_OBJECT(parent), "printer_combo"));
        GtkComboBoxText *paper_source_combo = GTK_COMBO_BOX_TEXT(g_object_get_data(G_OBJECT(parent), "paper_source_combo"));
        GtkCheckButton *reverse_check = GTK_CHECK_BUTTON(g_object_get_data(G_OBJECT(parent), "reverse_check"));
        
        if (!printer_combo || !paper_source_combo || !reverse_check) {
            std::cerr << "Error: One or more widgets not initialized properly!\n";
            return;
        }
        
        const char *printer = gtk_combo_box_text_get_active_text(printer_combo);
        const char *paper_source = gtk_combo_box_text_get_active_text(paper_source_combo);
        gboolean reverse = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(reverse_check));
        
        if (!printer || strcmp(printer, "No Printers Available") == 0) {
            std::cerr << "Error: No printer selected!\n";
            return;
        }
        
        GtkFileChooserNative *native = gtk_file_chooser_native_new("Pilih PDF", NULL, GTK_FILE_CHOOSER_ACTION_OPEN, "Open", "Cancel");
        if (gtk_native_dialog_run(GTK_NATIVE_DIALOG(native)) == GTK_RESPONSE_ACCEPT) {
            GtkFileChooser *chooser = GTK_FILE_CHOOSER(native);
            char *file_path = gtk_file_chooser_get_filename(chooser);
            if (!file_path) {
                std::cerr << "Error: No file selected!\n";
                return;
            }
            
            std::string command = "lp -d " + std::string(printer) + " -o sides=two-sided-long-edge";
            if (reverse) command += " -o outputorder=reverse";
            if (paper_source && *paper_source) command += " -o InputSlot=" + std::string(paper_source);
            command += " \"" + std::string(file_path) + "\"";
            
            std::cout << "Executing command: " << command << std::endl;
            std::system(command.c_str());
            g_free(file_path);
        }
        gtk_native_dialog_destroy(GTK_NATIVE_DIALOG(native));
    }

    static void activate(GtkApplication *app, gpointer user_data) {
        GtkWidget *window = gtk_application_window_new(app);
        gtk_window_set_title(GTK_WINDOW(window), "PDF Printer");
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 250);
        
        GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
        gtk_container_add(GTK_CONTAINER(window), box);
        
        GtkWidget *label = gtk_label_new("Pilih printer:");
        gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 5);
        
        GtkWidget *printer_combo = gtk_combo_box_text_new();
        FILE *fp = popen("lpstat -p | awk '{print $2}'", "r");
        char buffer[128];
        bool has_printer = false;
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            buffer[strcspn(buffer, "\n")] = 0;
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(printer_combo), buffer);
            has_printer = true;
        }
        pclose(fp);
        if (!has_printer) {
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(printer_combo), "No Printers Available");
        } else {
            gtk_combo_box_set_active(GTK_COMBO_BOX(printer_combo), 0);
        }
        gtk_box_pack_start(GTK_BOX(box), printer_combo, FALSE, FALSE, 5);
        
        GtkWidget *label2 = gtk_label_new("Pilih sumber kertas:");
        gtk_box_pack_start(GTK_BOX(box), label2, FALSE, FALSE, 5);
        
        GtkWidget *paper_source_combo = gtk_combo_box_text_new();
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(paper_source_combo), "Auto");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(paper_source_combo), "Tray1");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(paper_source_combo), "Tray2");
        gtk_combo_box_set_active(GTK_COMBO_BOX(paper_source_combo), 0);
        gtk_box_pack_start(GTK_BOX(box), paper_source_combo, FALSE, FALSE, 5);
        
        GtkWidget *reverse_check = gtk_check_button_new_with_label("Cetak terbalik");
        gtk_box_pack_start(GTK_BOX(box), reverse_check, FALSE, FALSE, 5);
        
        GtkWidget *btn_print = gtk_button_new_with_label("Cetak");
        g_signal_connect(btn_print, "clicked", G_CALLBACK(print_pdf), window);
        gtk_box_pack_start(GTK_BOX(box), btn_print, FALSE, FALSE, 5);
        
        g_object_set_data(G_OBJECT(window), "printer_combo", printer_combo);
        g_object_set_data(G_OBJECT(window), "paper_source_combo", paper_source_combo);
        g_object_set_data(G_OBJECT(window), "reverse_check", reverse_check);
        
        gtk_widget_show_all(window);
    }
};

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("com.example.PDFPrinter", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(PDFPrinter::activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
