package dev.tanakornsss.calorisense.util

import android.content.Context
import android.database.Cursor
import android.net.Uri
import android.provider.OpenableColumns
import java.io.File

fun copyModelFile(context: Context, uri: Uri): File {
    if (!isValidModelFile(context, uri)) throw IllegalArgumentException("This is not a .gguf file")
    val inputStream = context.contentResolver.openInputStream(uri)!!
    val outFile = File(context.filesDir, getFileName(context, uri) ?: "placeholder.gguf")
    inputStream.use { input ->
        outFile.outputStream().use { output ->
            input.copyTo(output)
        }
    }
    return outFile
}

private fun getFileName(context: Context, uri: Uri): String? {
    var name: String? = null
    val cursor: Cursor? = context.contentResolver.query(
        uri,
        null,
        null,
        null,
        null
    )
    cursor?.use {
        if (it.moveToFirst()) {
            name = it.getString(it.getColumnIndexOrThrow(OpenableColumns.DISPLAY_NAME))
        }
    }
    return name
}

private fun isValidModelFile(context: Context, uri: Uri): Boolean {
    val fileName = getFileName(context, uri) ?: return false
    return fileName.lowercase().endsWith(".gguf")
}