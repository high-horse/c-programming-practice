#include <poppler/glib/poppler.h>
#include <glib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <pdf-file>\n", argv[0]);
        return 1;
    }

    GError *error = NULL;
    gchar *uri = g_filename_to_uri(argv[1], NULL, &error);
    if (!uri) {
        fprintf(stderr, "Error converting filename: %s\n", error->message);
        g_error_free(error);
        return 1;
    }

    PopplerDocument *doc = poppler_document_new_from_file(uri, NULL, &error);
    g_free(uri);

    if (!doc) {
        fprintf(stderr, "Error opening PDF: %s\n", error->message);
        g_error_free(error);
        return 1;
    }

    int n_pages = poppler_document_get_n_pages(doc);
    FILE *out = fopen("output.txt", "w");
    if (!out) {
        perror("fopen");
        g_object_unref(doc);
        return 1;
    }

    for (int i = 0; i < n_pages; i++) {
        PopplerPage *page = poppler_document_get_page(doc, i);
        if (!page) continue;

        gchar *text = poppler_page_get_text(page);
        if (text) {
            fprintf(out, "%s\n", text);
            g_free(text);
        }
        g_object_unref(page);
    }

    fclose(out);
    g_object_unref(doc);
    return 0;
}
