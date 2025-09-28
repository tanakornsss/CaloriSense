package dev.tanakornsss.calorisense.ui.screen

import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.ExperimentalMaterial3ExpressiveApi
import androidx.compose.material3.Scaffold
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Devices.PIXEL_9
import androidx.compose.ui.tooling.preview.Preview

@OptIn(ExperimentalMaterial3ExpressiveApi::class)
@Composable
fun NewTokenGenScreen() {
    Scaffold { innerPadding ->
        Column(modifier = Modifier.padding(innerPadding)) {

        }
    }
}

@Composable
@Preview(device = PIXEL_9, showSystemUi = true)
private fun NewTokenGenScreenPreview() {
    NewTokenGenScreen()
}