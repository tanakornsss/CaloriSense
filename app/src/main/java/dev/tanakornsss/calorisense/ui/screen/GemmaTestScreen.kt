package dev.tanakornsss.calorisense.ui.screen

import androidx.activity.ComponentActivity
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
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.alpha
import androidx.compose.ui.unit.dp
import dev.tanakornsss.calorisense.handleTextTokens
import dev.tanakornsss.calorisense.returnOutputTokens

@Composable
fun GemmaTestScreen(activity: ComponentActivity) {
    var inputTokenText by remember { mutableStateOf("") }
    val canSubmit = remember { derivedStateOf { inputTokenText.isNotEmpty() } }.value
    // TODO : Implement loading model

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
                                    onClick = { handleTextTokens(inputTokenText)
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
            Row(
                horizontalArrangement = Arrangement.End,
                modifier = Modifier.fillMaxWidth()
            ) {
                Button(onClick = { }) {
                    Text("Load model")
                }
            }
            Spacer(modifier = Modifier.height(28.dp))
            Text(returnOutputTokens())
        }
    }
}
