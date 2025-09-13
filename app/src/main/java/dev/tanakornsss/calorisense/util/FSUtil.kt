package dev.tanakornsss.calorisense.util

import android.content.Context
import android.database.Cursor
import android.net.Uri
import android.provider.OpenableColumns
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import java.io.File

fun copyModelFileAsync(context: Context, uri: Uri, onComplete: (File?) -> Unit) {
    CoroutineScope(Dispatchers.IO).launch {
        try {
            if (!isValidModelFile(context, uri)) throw IllegalArgumentException("This is not a .gguf file")
            val inputStream = context.contentResolver.openInputStream(uri)!!
            val outFile = File(context.filesDir, getFileName(context, uri) ?: "placeholder.gguf")
            inputStream.use { input ->
                outFile.outputStream().use { output ->
                    input.copyTo(output)
                }
            }
            withContext(Dispatchers.Main) {
                onComplete(outFile)
            }
        }
        catch (e: Exception) {
            withContext(Dispatchers.Main) {
                onComplete(null)
            }
        }
    }
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