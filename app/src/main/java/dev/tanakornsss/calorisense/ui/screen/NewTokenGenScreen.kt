package dev.tanakornsss.calorisense.ui.screen

import androidx.compose.foundation.isSystemInDarkTheme
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.text.BasicTextField
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.automirrored.filled.Send
import androidx.compose.material3.Button
import androidx.compose.material3.Card
import androidx.compose.material3.ExperimentalMaterial3ExpressiveApi
import androidx.compose.material3.Icon
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.OutlinedIconButton
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.SolidColor
import androidx.compose.ui.text.TextStyle
import androidx.compose.ui.tooling.preview.Devices.PIXEL_9
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp

@Composable
fun NewTokenGenScreen() {
    Scaffold { innerPadding ->
        Column(
            modifier = Modifier
                .fillMaxSize()
                .padding(innerPadding)
                .padding(horizontal = 20.dp)
        ) {
            Spacer(modifier = Modifier.weight(1f))
            FloatingChatBar()
        }
    }
}

@OptIn(ExperimentalMaterial3ExpressiveApi::class)
@Composable
private fun FloatingChatBar() {
    var textFieldState by remember { mutableStateOf("") }
    val isDarkMode = isSystemInDarkTheme()
    val color = if (isDarkMode) Color.White else Color.Black
    val textStyle = TextStyle(color = color)

    Card(
        shape = MaterialTheme.shapes.largeIncreased,
        modifier = Modifier
            .fillMaxWidth()
            .height(150.dp)
    ) {
        Column(
            verticalArrangement = Arrangement.SpaceBetween,
            modifier = Modifier
                .padding(20.dp)
                .fillMaxSize()
        ) {
            Row(
                horizontalArrangement = Arrangement.SpaceBetween,
                modifier = Modifier.fillMaxWidth()
            ) {
                BasicTextField(
                    value = textFieldState,
                    onValueChange = {
                        textFieldState = it
                    },
                    textStyle = textStyle,
                    cursorBrush = SolidColor(color),
                    decorationBox = { innerTextField ->
                        Box {
                            if (textFieldState.isEmpty()) Text(
                                text = "Enter text",
                                style = textStyle,
                                color = Color.Gray
                            )
                            innerTextField()
                        }
                    }
                )
                OutlinedIconButton(onClick = { }) {
                    Icon(imageVector = Icons.AutoMirrored.Filled.Send, contentDescription = null)
                }
            }
            // TODO: Change to icon for more compact look
            Row(modifier = Modifier.fillMaxWidth()) {
                Button(onClick = { }) { Text("Select model") }
                Button(onClick = { }) { Text("Add image") }
            }
        }
    }
}

@Composable
@Preview(device = PIXEL_9, showSystemUi = true)
private fun NewTokenGenScreenPreview() {
    NewTokenGenScreen()
}