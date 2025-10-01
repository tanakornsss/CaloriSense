package dev.tanakornsss.calorisense.ui.nav

import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.imePadding
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.material3.HorizontalDivider
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Devices.PIXEL_9
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp

@Composable
fun ManageModelScreen() {
    Scaffold { innerPadding ->
        Column(modifier = Modifier
            .fillMaxSize()
            .padding(innerPadding)
            .padding(horizontal = 20.dp)
            .imePadding()
        ) {
            LazyColumn {
                items(5) {
                    ModelCard()
                }
            }
        }
    }
}

@Composable
private fun ModelCard() {
    Column(
        modifier = Modifier
            .fillMaxWidth()
            .height(100.dp)
    ) {
        Text("Hello world")
        Spacer(modifier = Modifier.weight(1f))
        HorizontalDivider()
    }
}

@Composable
@Preview(device = PIXEL_9, showSystemUi = true)
fun ManageModelScreenPreview() {
    ManageModelScreen()
}