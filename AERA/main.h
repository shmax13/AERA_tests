//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//_/_/
//_/_/ AERA
//_/_/ Autocatalytic Endogenous Reflective Architecture
//_/_/ 
//_/_/ Copyright (c) 2023-2026 Jeff Thompson
//_/_/ Copyright (c) 2023-2026 Kristinn R. Thorisson
//_/_/ Copyright (c) 2018-2026 Chloe Schaff
//_/_/ Copyright (c) 2023-2026 Icelandic Institute for Intelligent Machines
//_/_/ http://www.iiim.is
//_/_/ 
//_/_/ Copyright (c) 2010-2012 Eric Nivel
//_/_/ Center for Analysis and Design of Intelligent Agents
//_/_/ Reykjavik University, Menntavegur 1, 102 Reykjavik, Iceland
//_/_/ http://cadia.ru.is
//_/_/ 
//_/_/ Part of this software was developed by Eric Nivel
//_/_/ in the HUMANOBS EU research project, which included
//_/_/ the following parties:
//_/_/
//_/_/ Autonomous Systems Laboratory
//_/_/ Technical University of Madrid, Spain
//_/_/ http://www.aslab.org/
//_/_/
//_/_/ Communicative Machines
//_/_/ Edinburgh, United Kingdom
//_/_/ http://www.cmlabs.com/
//_/_/
//_/_/ Istituto Dalle Molle di Studi sull'Intelligenza Artificiale
//_/_/ University of Lugano and SUPSI, Switzerland
//_/_/ http://www.idsia.ch/
//_/_/
//_/_/ Institute of Cognitive Sciences and Technologies
//_/_/ Consiglio Nazionale delle Ricerche, Italy
//_/_/ http://www.istc.cnr.it/
//_/_/
//_/_/ Dipartimento di Ingegneria Informatica
//_/_/ University of Palermo, Italy
//_/_/ http://diid.unipa.it/roboticslab/
//_/_/
//_/_/
//_/_/ --- HUMANOBS Open-Source BSD License, with CADIA Clause v 1.0 ---
//_/_/
//_/_/ Redistribution and use in source and binary forms, with or without
//_/_/ modification, is permitted provided that the following conditions
//_/_/ are met:
//_/_/ - Redistributions of source code must retain the above copyright
//_/_/   and collaboration notice, this list of conditions and the
//_/_/   following disclaimer.
//_/_/ - Redistributions in binary form must reproduce the above copyright
//_/_/   notice, this list of conditions and the following disclaimer 
//_/_/   in the documentation and/or other materials provided with 
//_/_/   the distribution.
//_/_/
//_/_/ - Neither the name of its copyright holders nor the names of its
//_/_/   contributors may be used to endorse or promote products
//_/_/   derived from this software without specific prior 
//_/_/   written permission.
//_/_/   
//_/_/ - CADIA Clause: The license granted in and to the software 
//_/_/   under this agreement is a limited-use license. 
//_/_/   The software may not be used in furtherance of:
//_/_/    (i)   intentionally causing bodily injury or severe emotional 
//_/_/          distress to any person;
//_/_/    (ii)  invading the personal privacy or violating the human 
//_/_/          rights of any person; or
//_/_/    (iii) committing or preparing for any act of war.
//_/_/
//_/_/ THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
//_/_/ CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
//_/_/ INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
//_/_/ MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
//_/_/ DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
//_/_/ CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
//_/_/ SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
//_/_/ BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
//_/_/ SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
//_/_/ INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
//_/_/ WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
//_/_/ NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
//_/_/ OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
//_/_/ OF SUCH DAMAGE.
//_/_/ 
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

#ifndef main_h
#define main_h

#include <map>
#include "../submodules/CoreLibrary/CoreLibrary/types.h"
#include "test_mem.h"
#include "../r_exec/init.h"
#include "../r_comp/decompiler.h"
#include "settings.h"

core::int32 start_AERA(const char* file_name, const char* decompiled_file_name);

/**
 * AERA_interface supports running from the AERA Visualizer.
 */
class AERA_interface {
public:
	// Create a new instance and set it up
	AERA_interface(const char* settings_file_name, const char* decompiled_file_name);

	// Step the diagnostic time state once
	bool step() {
		return diagnostic_time_state_->step();
	}

	// Run in diagnostic time for a certain amount of time
	// Returns false when AERA has run to the end and true otherwise
	bool runFor(std::chrono::milliseconds time_step);

	// Run AERA all the way to settings->run_time_
	void run();

	/**
	 * Dump everything AERA's working on to a file for analysis.
	 * \param objectLabel (optional) A map of a Code* object to a label. The object is matched by detail_oid.
	 */
	void brainDump(const std::map<r_code::Code*, std::string>* objectLabel = NULL);

	// Shuts everything down
	void stop();

	// Return an image of AERA's current models
	r_comp::Image getModelsImage() {
		r_comp::Image image = *mem_->get_models();
		image.object_names_.symbols_ = r_exec::Seed.object_names_.symbols_;
		return image;
	}

	// Return an image of AERA's current objects
	r_comp::Image* getObjectsImage() {
		r_comp::Image* image = mem_->get_objects(settings_->keep_invalidated_objects_);
		image->object_names_.symbols_ = r_exec::Seed.object_names_.symbols_; // Set opcode names
		return image;
	}

	// Return the names used in the seed program
	r_comp::ObjectNames getSeedNames() {
		return r_exec::Seed.object_names_;
	}

	// Return the metadata used to interpret the image
	r_comp::Metadata getMetadata() {
		return r_exec::Metadata;
	}

	// Return start time for Visualizer
	core::Timestamp getStartTime() {
		return starting_time_;
	}

	// Return start time for Visualizer
	core::Timestamp getCurrentTime() {
		return r_exec::Now();
	}

	// Return the settings file for use in the Visualizer
	Settings* getSettings() {
		return settings_;
	}

	// Return a link to mem_ for the Visualizer's TaskEnvironmentView
	TestMem<r_exec::LObject, r_exec::MemStatic>* getMem() {
		return (TestMem<r_exec::LObject, r_exec::MemStatic>*) mem_;
	}


private:
	r_comp::Decompiler decompiler_;
	r_exec::_Mem* mem_;
	r_code::resized_vector<r_code::Code*> ram_objects_;
	r_exec::DiagnosticTimeState* diagnostic_time_state_;
	std::ofstream runtime_output_stream_;
	uint32 stdin_oid_;
	uint32 stdout_oid_;
	uint32 self_oid_;

	const char* settings_file_name_;
	const char* decompiled_file_name_;

	Settings* settings_;
	core::Timestamp starting_time_;
	std::chrono::milliseconds current_time_;
};

#endif
