package dev.tanakornsss.calorisense.util

import android.content.Context
import androidx.datastore.preferences.core.edit
import androidx.datastore.preferences.core.stringPreferencesKey
import androidx.datastore.preferences.preferencesDataStore
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.map

val Context.dataStore by preferencesDataStore(name = "model_prefs")

object ModelPrefs {
    val LAST_MODEL = stringPreferencesKey("last_model")
}

suspend fun saveModelFileName(context: Context, fileName: String) {
    context.dataStore.edit { prefs ->
        prefs[ModelPrefs.LAST_MODEL] = fileName
    }
}

fun readModelFileName(context: Context): Flow<String?> {
    return context.dataStore.data.map { prefs ->
        prefs[ModelPrefs.LAST_MODEL]
    }
}