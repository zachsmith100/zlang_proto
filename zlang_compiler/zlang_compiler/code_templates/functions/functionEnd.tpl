	} catch (std::exception& e) { Log.Warnings.AddLine2s("Function [[functionName]] interrupted after line %s: %s", currentLine, e.what()); return getResultObject(false); }; return result;
