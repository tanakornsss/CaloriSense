package dev.tanakornsss.calorisense.ui.screen

import android.content.Context
import android.net.Uri
import android.widget.Toast
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
import androidx.compose.material3.LinearProgressIndicator
import androidx.compose.material3.OutlinedTextField
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.collectAsState
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
import dev.tanakornsss.calorisense.util.readModelFileName
import dev.tanakornsss.calorisense.util.saveModelFileName
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext

@Composable
fun TokenGenScreen(context: Context) {
    var inputTokenText by remember { mutableStateOf("") }
    var isModelLoaded by remember { mutableStateOf(false) }

    var copyJob by remember { mutableStateOf<Job?>(null) }
    var isCopying by remember { mutableStateOf(false) }
    var isLoadingModelFromCache by remember { mutableStateOf(false) }
    var loadModelProgress by remember { mutableIntStateOf(0) }
    var isGeneratingResponse by remember { mutableStateOf(false) }

    val isLoading by remember { derivedStateOf { isLoadingModelFromCache || isGeneratingResponse } }
    val canSubmitText by remember { derivedStateOf { isModelLoaded && inputTokenText.isNotEmpty() && !isLoading } }

    val lastModelPath by readModelFileName(context).collectAsState(initial = "")

    LaunchedEffect(lastModelPath) {
        isLoadingModelFromCache = true
        withContext(Dispatchers.IO) {
            lastModelPath?.let { path ->
                isModelLoaded = loadModel(path)
            }
        }

        if (isModelLoaded) {
            Toast.makeText(
                context,
                "Model has been loaded",
                Toast.LENGTH_SHORT
            ).show()
        }
        isLoadingModelFromCache = false
    }

    if (isCopying) LoadModelDialog(loadModelProgress) {
        copyJob?.cancel()
        isCopying = false
        copyJob = null
    }

    val pickModel = rememberLauncherForActivityResult(
        contract = ActivityResultContracts.OpenDocument()
    ) { uri: Uri? ->
        uri?.let {
            isCopying = true
            copyJob = copyModelFile(
                context = context,
                uri = it,
                scope = CoroutineScope(Dispatchers.IO),
                onComplete = { file ->
                    if (file != null) {
                        val filePath = file.absolutePath
                        val success = loadModel(filePath)

                        CoroutineScope(Dispatchers.Main).launch {
                            isModelLoaded = success
                            Toast.makeText(
                                context,
                                if (success) "File ${file.name} is loaded"
                                else "Failed to load ${file.name}",
                                Toast.LENGTH_SHORT
                            ).show()
                        }

                        CoroutineScope(Dispatchers.IO).launch {
                            saveModelFileName(context, filePath)
                        }

                        isCopying = false
                        copyJob = null
                    }
                },
                onProgress = { p ->
                    loadModelProgress = p
                }
            )
        }
    }

    Scaffold { innerPadding ->
        if (isLoading) LinearProgressIndicator(modifier = Modifier.fillMaxWidth())
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
                                    enabled = canSubmitText,
                                    onClick = {
                                        isGeneratingResponse = true
                                        CoroutineScope(Dispatchers.IO).launch {
                                            handleTextTokens(inputTokenText)
                                            withContext(Dispatchers.Main) {
                                                isGeneratingResponse = false
                                            }
                                        }
                                    })
                                .alpha(if (!canSubmitText) 0.5f else 1.0f)
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
private fun LoadModelDialog(
    progress: Int,
    onDismiss: () -> Unit
) {
    ProgressDialog(
        onDismissRequest = { onDismiss() },
        dialogTitle = "Loading model",
        progress = progress
    )
}
