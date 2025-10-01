package dev.tanakornsss.calorisense

import androidx.activity.ComponentActivity
import androidx.compose.material3.Surface
import androidx.compose.runtime.Composable
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.compose.rememberNavController
import dev.tanakornsss.calorisense.ui.nav.CaloriSenseScreen
import dev.tanakornsss.calorisense.ui.nav.ManageModelScreen
import dev.tanakornsss.calorisense.ui.nav.NewTokenGenScreen
import dev.tanakornsss.calorisense.ui.nav.TokenGenScreen
import dev.tanakornsss.calorisense.ui.theme.CaloriSenseTheme

@Composable
fun CaloriSenseApp(context: ComponentActivity) {
    val navController = rememberNavController()
    
    CaloriSenseTheme {
        Surface {
            NavHost(
                navController = navController,
                startDestination = CaloriSenseScreen.NewTokenGen.name,
            ) {
                composable(CaloriSenseScreen.TokenGen.name) {
                    TokenGenScreen(context)
                }
                composable(CaloriSenseScreen.NewTokenGen.name) {
                    NewTokenGenScreen {
                        navController.navigate(CaloriSenseScreen.ManageModel.name)
                    }
                }
                composable(CaloriSenseScreen.ManageModel.name) {
                    ManageModelScreen()
                }
            }
        }
    }
}