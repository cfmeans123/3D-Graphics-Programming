#pragma once
#include "Skeleton.h"


namespace MEngine::Graphics
{
    struct Model;
    class Animation;

	class IKSolver
	{
	public:
		IKSolver() = default;
		virtual ~IKSolver() = default;

		/// @brief Interface to solve the inverse kinematics problem
		/// @param skeleton The skeleton to solve
		/// @param targetPos The target position to reach
		/// @param maxIterations The maximum number of iterations to solve the problem
		/// @param epsilon The maximum deviation from the target position
		/// @return Returns true if the end effector is within epsilon from the target before max iterations are reached else returns false
		bool SolveIK(Skeleton& skeleton, const Math::Vector3& targetPos, int maxIterations, float epsilon);
	};



}
