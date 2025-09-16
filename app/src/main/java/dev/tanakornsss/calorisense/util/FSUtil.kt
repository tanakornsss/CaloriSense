package dev.tanakornsss.calorisense.util

import android.content.Context
import android.database.Cursor
import android.net.Uri
import android.provider.OpenableColumns
import android.widget.Toast
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import java.io.File

fun copyModelFile(
    context: Context,
    uri: Uri,
    scope: CoroutineScope,
    onComplete: (File?) -> Unit,
    onProgress: (Int) -> Unit
): Job {
    return scope.launch(Dispatchers.IO) {
        try {
            if (!isValidModelFile(context, uri)) {
                withContext(Dispatchers.Main) {
                    Toast.makeText(
                        context,
                        "Invalid file type, please reselect",
                        Toast.LENGTH_SHORT
                    ).show()
                    onComplete(null)
                }
                return@launch
            }

            val fileName = getFileName(context, uri) ?: "placeholder.gguf"
            val outFile = File(context.filesDir, fileName)

            val totalSize = context.contentResolver.openFileDescriptor(uri, "r")?.use {
                it.statSize
            } ?: -1L

            val inputStream = context.contentResolver.openInputStream(uri)
            if (inputStream == null) {
                withContext(Dispatchers.Main) {
                    onComplete(null)
                }
                return@launch
            }

            val buffer = ByteArray(DEFAULT_BUFFER_SIZE)
            var bytesCopied: Long = 0

            inputStream.use { input ->
                outFile.outputStream().use { output ->
                    var bytes = input.read(buffer)

                    while (bytes >= 0) {
                        output.write(buffer, 0, bytes)
                        bytesCopied += bytes

                        if (totalSize > 0) {
                            val progress = (bytesCopied * 100 / totalSize).toInt()
                            withContext(Dispatchers.Main) {
                                onProgress(progress)
                            }
                        }

                        bytes = input.read(buffer)
                    }
                }
            }

            withContext(Dispatchers.Main) {
                onComplete(outFile)
            }
        }
        catch (_: Exception) {
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