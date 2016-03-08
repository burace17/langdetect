#include "gtk/gtk_gui.h"
GtkWidget* window;
GtkWidget* input;
char stop_words_dir_path[BUFSIZ];
void _gtk_display_dialog(char output[]) {
	GtkWidget* dialog = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, output, "Results");
	gtk_window_set_title(GTK_WINDOW(dialog), "Results");
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

void select_button_click(GtkWidget* widget, gpointer data) {
	GtkWidget* dialog;
	char* filename;
	dialog = gtk_file_chooser_dialog_new("Open Stop Word Folder",
																				GTK_WINDOW(window),
																				GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
																				GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
																				GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);

	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		strncpy(stop_words_dir_path, filename, sizeof(stop_words_dir_path));
		g_free(filename);	
	}

	gtk_widget_destroy(dialog);
}

void detect_button_click(GtkWidget* widget, gpointer data) {
	GtkTextIter start;
	GtkTextIter end;
	char* text;
	GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(input));
	
	gtk_text_buffer_get_start_iter(buffer, &start);
	gtk_text_buffer_get_end_iter(buffer, &end);

	text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

	detect_language(stop_words_dir_path, text);
	g_free(text);
}
void _gtk_create_window(int argc, char* argv[]) {
	GtkWidget* button;
	GtkWidget* stop_button;
	GtkWidget* frame;
	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Language Detection");
	gtk_window_set_default_size(GTK_WINDOW(window), 480, 375);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

	frame = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(window), frame);


	button = gtk_button_new_with_label("Detect Language");
	gtk_widget_set_size_request(button, 215, 27);
	gtk_fixed_put(GTK_FIXED(frame), button, 137, 290);

	stop_button = gtk_button_new_with_label("Select Stop Files Directory");
	gtk_widget_set_size_request(stop_button, 215, 27);
	gtk_fixed_put(GTK_FIXED(frame), stop_button, 137, 325);

	input = gtk_text_view_new();
	gtk_widget_set_size_request(input, 475, 275);
	gtk_fixed_put(GTK_FIXED(frame), input, 5, 0);

	gtk_widget_show_all(window);

	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(button, "clicked", G_CALLBACK(detect_button_click), NULL);
	g_signal_connect(stop_button, "clicked", G_CALLBACK(select_button_click), NULL);

	gtk_main();

}

