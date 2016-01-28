#!/usr/bin/tclsh

# Copyright 2016 Samsung Electronics Co., Ltd.
# Copyright 2016 University of Szeged.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

foreach f [getSourceFileNames] {
    set lineNumber 1
    foreach line [getAllLines $f] {
        if {[regexp {\w\*\s\w+} $line]} {
            report $f $lineNumber "pointer dereference operator always should be aligned to right."
        }
        incr lineNumber
    }
}
