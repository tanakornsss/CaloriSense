package dev.tanakornsss.calorisense.ui.screen

import android.net.Uri
import android.widget.Toast
import androidx.activity.ComponentActivity
import androidx.activity.compose.rememberLauncherForActivityResult
import androidx.activity.result.contract.ActivityResultContracts
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.automirrored.filled.Send
import androidx.compose.material3.Button
import androidx.compose.material3.Icon
import androidx.compose.material3.OutlinedTextField
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.derivedStateOf
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableIntStateOf
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.alpha
import androidx.compose.ui.unit.dp
import dev.tanakornsss.calorisense.handleTextTokens
import dev.tanakornsss.calorisense.loadModel
import dev.tanakornsss.calorisense.returnOutputTokens
import dev.tanakornsss.calorisense.ui.component.ProgressDialog
import dev.tanakornsss.calorisense.util.copyModelFile
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job

@Composable
fun GemmaTestScreen(activity: ComponentActivity) {
    var inputTokenText by remember { mutableStateOf("") }
    val canSubmit = remember { derivedStateOf { inputTokenText.isNotEmpty() } }.value

    var copyJob by remember { mutableStateOf<Job?>(null) }
    var isCopying by remember { mutableStateOf(false) }
    var copyModelProgress by remember { mutableIntStateOf(0) }

    if (isCopying) CopyingModelDialog(copyModelProgress) {
        copyJob?.cancel()
        isCopying = false
        copyJob = null
    }

    // TODO : Pass the model to C++ side
    val pickModel = rememberLauncherForActivityResult(
        contract = ActivityResultContracts.OpenDocument()
    ) { uri: Uri? ->
        uri?.let {
            isCopying = true
            copyJob = copyModelFile(
                context = activity,
                uri = it,
                scope = CoroutineScope(Dispatchers.IO),
                onComplete = { file ->
                    if (file != null) {
                        val success = loadModel(file.absolutePath)
                        Toast.makeText(
                            activity,
                            if (success) "File ${file.name} is copied"
                            else "Failed to load ${file.name}",
                            Toast.LENGTH_SHORT
                        ).show()
                    }
                    isCopying = false
                    copyJob = null
                },
                onProgress = { p ->
                    copyModelProgress = p
                }
            )
        }
    }

    Scaffold { innerPadding ->
        Column(
            modifier = Modifier
                .fillMaxSize()
                .padding(innerPadding)
                .padding(horizontal = 20.dp)
        ) {
            OutlinedTextField(
                label = { Text("Input your prompt") },
                trailingIcon = {
                    Box(
                        modifier =
                            Modifier
                                .clickable(
                                    enabled = canSubmit,
                                    onClick = {
                                        handleTextTokens(inputTokenText)
                                    })
                                .alpha(if (!canSubmit) 0.5f else 1.0f)
                                .padding(8.dp)
                    ) {
                        Icon(
                            imageVector = Icons.AutoMirrored.Filled.Send,
                            contentDescription = null
                        )
                    }
                },
                value = inputTokenText,
                onValueChange = { inputTokenText = it },
                modifier = Modifier.fillMaxWidth()
            )
            Spacer(modifier = Modifier.height(14.dp))
            Column {
                Row(
                    horizontalArrangement = Arrangement.End,
                    modifier = Modifier.fillMaxWidth()
                ) {
                    Button(onClick = { pickModel.launch(arrayOf("application/octet-stream")) }) {
                        Text("Load model")
                    }
                }
                Spacer(modifier = Modifier.height(20.dp))
            }
            Spacer(modifier = Modifier.height(28.dp))
            Text(returnOutputTokens())
        }
    }
}

@Composable
private fun CopyingModelDialog(
    progress: Int,
    onDismiss: () -> Unit
) {
    ProgressDialog(
        onDismissRequest = { onDismiss() },
        dialogTitle = "Copying model",
        progress = progress
    )
}
